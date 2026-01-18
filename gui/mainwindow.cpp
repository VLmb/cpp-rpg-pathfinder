#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGraphicsLineItem>
#include <QMenu>
#include <QDebug>
#include <QFile>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
      , scene(new QGraphicsScene(this))
      , mapPixmapItem(nullptr)
      , selectionRect(nullptr)
      , manager(nullptr)
      , currentHero(nullptr)
      , userGridWidth(0)
      , userGridHeight(0)
      , cellStepX(1.0)
      , cellStepY(1.0)
      , currentState(AppState::IDLE)
      , isCastMode(false)
{
    ui->setupUi(this);

    ui->mapView->setScene(scene);
    ui->mapView->viewport()->installEventFilter(this);

    ui->mapView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->mapView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    ui->mapView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mapView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->comboHero->clear();
    ui->comboHero->addItem("");

    Human tempHuman;
    ui->comboHero->addItem(QString::fromStdString(tempHuman.getHeroName()));

    WoodElf tempElf;
    ui->comboHero->addItem(QString::fromStdString(tempElf.getHeroName()));

    Orc tempOrc;
    ui->comboHero->addItem(QString::fromStdString(tempOrc.getHeroName()));

    Gnome tempGnome;
    ui->comboHero->addItem(QString::fromStdString(tempGnome.getHeroName()));

    ui->comboHero->setCurrentIndex(0);

    setupLegend();

    ui->lblStatus->setText("Готово к работе. Сгенерируйте карту.");
}

MainWindow::~MainWindow()
{
    delete ui;
    if (manager) delete manager;
    if (currentHero) delete currentHero;
}

void MainWindow::reloadMapImage()
{
    scene->clear();
    mapPixmapItem = nullptr;
    selectionRect = nullptr;

    QImage image;
    if (!image.load(QString::fromStdString(mapFileName))) {
        return;
    }

    int imgW = image.width();
    int imgH = image.height();

    mapPixmapItem = scene->addPixmap(QPixmap::fromImage(image));
    mapPixmapItem->setZValue(0);

    if (userGridWidth > 0 && userGridHeight > 0) {
        cellStepX = (double)imgW / userGridWidth;
        cellStepY = (double)imgH / userGridHeight;
    }

    drawOverlayGrid(imgW, imgH);
    drawVisualEdges();

    scene->setSceneRect(0, 0, imgW, imgH);
    ui->mapView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::drawOverlayGrid(int imgW, int imgH)
{
    QPen gridPen(Qt::black);
    gridPen.setWidth(0);
    gridPen.setColor(QColor(0, 0, 0, 120));

    for (int i = 0; i <= userGridWidth; ++i) {
        double x = i * cellStepX;
        QGraphicsLineItem *l = scene->addLine(x, 0, x, imgH, gridPen);
        l->setZValue(1);
    }
    for (int i = 0; i <= userGridHeight; ++i) {
        double y = i * cellStepY;
        QGraphicsLineItem *l = scene->addLine(0, y, imgW, y, gridPen);
        l->setZValue(1);
    }
}

void MainWindow::drawVisualEdges()
{
    if (!manager) return;

    QPen edgePen(Qt::red);
    edgePen.setWidth(1);

    auto adjList = manager->getCheckpointGraphAdjList();

    for (int u = 0; u < (int)adjList.size(); ++u) {
        for (int v : adjList[u]) {
            if (u < v) {
                auto p1 = manager->getPointOfIndex(u);
                auto p2 = manager->getPointOfIndex(v);

                double x1 = (p1.x * cellStepX) + (cellStepX / 2.0);
                double y1 = (p1.y * cellStepY) + (cellStepY / 2.0);
                double x2 = (p2.x * cellStepX) + (cellStepX / 2.0);
                double y2 = (p2.y * cellStepY) + (cellStepY / 2.0);

                QGraphicsLineItem *line = scene->addLine(x1, y1, x2, y2, edgePen);
                line->setZValue(2);
            }
        }
    }
}

void MainWindow::highlightCell(int gridX, int gridY)
{
    if (!selectionRect) {
        selectionRect = scene->addRect(0, 0, cellStepX, cellStepY, QPen(Qt::yellow, 2), QBrush(QColor(255, 255, 0, 50)));
        selectionRect->setZValue(3);
    }
    selectionRect->setRect(gridX * cellStepX, gridY * cellStepY, cellStepX, cellStepY);
    selectionRect->setVisible(true);
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->mapView->viewport()) {

        if (event->type() == QEvent::Wheel) {
            QWheelEvent *we = static_cast<QWheelEvent*>(event);
            const double scaleFactor = 1.15;
            if (we->angleDelta().y() > 0) {
                ui->mapView->scale(scaleFactor, scaleFactor);
            } else {
                ui->mapView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
            }
            return true;
        }

        if (event->type() == QEvent::MouseButtonPress) {
            if (!manager) return false;

            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            QPointF scenePos = ui->mapView->mapToScene(me->pos());

            int gridX = static_cast<int>(scenePos.x() / cellStepX);
            int gridY = static_cast<int>(scenePos.y() / cellStepY);

            if (gridX < 0 || gridX >= userGridWidth || gridY < 0 || gridY >= userGridHeight) {
                return false;
            }

            highlightCell(gridX, gridY);

            switch (currentState) {
                case AppState::IDLE:
                    ui->lblStatus->setText(QString("Выбрана клетка: %1, %2").arg(gridX).arg(gridY));
                    if (me->button() == Qt::RightButton) {
                        showContextMenu(me->globalPosition().toPoint(), gridX, gridY);
                    }
                    break;

                case AppState::SELECTING_EDGE_1:
                    tempPoint1 = QPoint(gridX, gridY);
                    currentState = AppState::SELECTING_EDGE_2;
                    ui->lblStatus->setText(QString("Связь: старт [%1, %2]. Жду финиш...").arg(gridX).arg(gridY));
                    break;

                case AppState::SELECTING_EDGE_2:
                    if (manager->addEdgeToCheckpointGraph(tempPoint1.x(), tempPoint1.y(), gridX, gridY)) {
                        ui->lblStatus->setText("Связь добавлена.");
                        reloadMapImage();
                    } else {
                        ui->lblStatus->setText("Ошибка добавления связи.");
                    }
                    resetState();
                    break;

                case AppState::SELECTING_REMOVE_EDGE_1:
                    tempPoint1 = QPoint(gridX, gridY);
                    currentState = AppState::SELECTING_REMOVE_EDGE_2;
                    ui->lblStatus->setText(QString("Удаление: старт [%1, %2]. Жду финиш...").arg(gridX).arg(gridY));
                    break;

                case AppState::SELECTING_REMOVE_EDGE_2:
                    if (manager->removeEdgeFromCheckpointGraph(tempPoint1.x(), tempPoint1.y(), gridX, gridY)) {
                        ui->lblStatus->setText("Связь удалена.");
                        reloadMapImage();
                    } else {
                        QMessageBox::warning(this, "Ошибка", "Не удалось удалить связь.\nВозможно, её не существует.");
                        ui->lblStatus->setText("Ошибка удаления связи.");
                    }
                    resetState();
                    break;

                case AppState::SELECTING_PATH_1:
                    tempPoint1 = QPoint(gridX, gridY);
                    currentState = AppState::SELECTING_PATH_2;
                    ui->lblStatus->setText(QString("Путь: старт [%1, %2]. Жду финиш...").arg(gridX).arg(gridY));
                    break;

                case AppState::SELECTING_PATH_2:
                    try {
                        if (!currentHero) throw std::runtime_error("Герой не выбран!");

                        double time = manager->findPathAndDraw(
                                tempPoint1.x(), tempPoint1.y(),
                                gridX, gridY,
                                *currentHero
                                );

                        if (std::isinf(time)) {
                            QMessageBox::warning(this, "Нет пути", "Не существует пути между этими вершинами!");
                            ui->lblStatus->setText("Путь не найден (бесконечность).");
                        } else {
                            QString heroName = QString::fromStdString(currentHero->getHeroName());
                            QString timeStr = QString::number(time, 'f', 2);
                            QString modeStr = isCastMode ? "(Cast)" : "(Natural)";

                            QString msg = QString("Маршрут построен %1!\n\n"
                                                  "Начало: [%2, %3]\n"
                                                  "Конец: [%4, %5]\n"
                                                  "Герой: %6\n"
                                                  "Время в пути: %7")
                                                  .arg(modeStr)
                                                  .arg(tempPoint1.x()).arg(tempPoint1.y())
                                                  .arg(gridX).arg(gridY)
                                                  .arg(heroName)
                                                  .arg(timeStr);

                            QMessageBox::information(this, "Результат поиска", msg);
                            ui->lblStatus->setText(QString("Путь найден: %1").arg(timeStr));
                        }

                        reloadMapImage();
                    } catch (const std::exception &e) {
                        ui->lblStatus->setText(QString("Ошибка: %1").arg(e.what()));
                        QMessageBox::critical(this, "Ошибка", e.what());
                    }
                    resetState();
                    break;
            }
            return true;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (scene && scene->width() > 0) {
        ui->mapView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    }
}

void MainWindow::showContextMenu(const QPoint &screenPos, int gridX, int gridY)
{
    QMenu menu;
    QAction *addAct = menu.addAction("Добавить вершину");
    QAction *delAct = menu.addAction("Удалить вершину");

    QAction *selected = menu.exec(screenPos);

    if (selected == addAct) {
        manager->addVertexToCheckpointGraph(gridX, gridY);
        ui->lblStatus->setText(QString("Вершина добавлена: %1, %2").arg(gridX).arg(gridY));
        reloadMapImage();
    }
    else if (selected == delAct) {
        if (manager->removeVertexFromCheckpointGraph(gridX, gridY)) {
            ui->lblStatus->setText("Вершина удалена.");
            reloadMapImage();
        } else {
            ui->lblStatus->setText("Ошибка удаления.");
        }
    }
}

void MainWindow::runGenerationDialog(bool isCast)
{
    QDialog dialog(this);
    QString title = isCast ? "Параметры генерации (Упрощенная)" : "Параметры генерации (Обычная)";
    dialog.setWindowTitle(title);

    QFormLayout form(&dialog);

    QSpinBox *spinW = new QSpinBox(&dialog); spinW->setRange(2, 2000); spinW->setValue(40);
    QSpinBox *spinH = new QSpinBox(&dialog); spinH->setRange(2, 2000); spinH->setValue(30);

    QDoubleSpinBox *spinScale = new QDoubleSpinBox(&dialog);
    spinScale->setRange(0.0, 500.0);
    spinScale->setSingleStep(0.1);
    spinScale->setValue(0.0f);

    QSpinBox *spinSeed = new QSpinBox(&dialog); spinSeed->setRange(0, 9999999); spinSeed->setValue(12345);

    form.addRow("Ширина:", spinW);
    form.addRow("Высота:", spinH);
    form.addRow("Scale (0=Auto):", spinScale);
    form.addRow("Seed:", spinSeed);

    QDialogButtonBox btnBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&btnBox);
    connect(&btnBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&btnBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        if (manager) delete manager;

        userGridWidth = spinW->value();
        userGridHeight = spinH->value();
        float scaleVal = static_cast<float>(spinScale->value());

        manager = new PathfinderManager(
                userGridWidth,
                userGridHeight,
                mapFileName,
                isCast,
                scaleVal,
                spinSeed->value()
                );

        manager->saveMapToFile();
        reloadMapImage();

        QString modeStr = isCast ? "(Упрощенная)" : "(Обычная)";
        ui->lblStatus->setText("Карта сгенерирована " + modeStr);
    }
}

void MainWindow::on_btnGenerateNatural_clicked()
{
    runGenerationDialog(false);
}

void MainWindow::on_btnGenerateCast_clicked()
{
    runGenerationDialog(true);
}

void MainWindow::on_btnGenerateGraph_clicked()
{
    if (!manager) {
        QMessageBox::information(this, "Инфо", "Сначала сгенерируйте карту.");
        return;
    }

    bool ok;
    int count = QInputDialog::getInt(this, "Генерация графа",
                                     "Количество вершин:", 5, 1, 10000, 1, &ok);
    if (ok) {
        manager->generateCheckpointGraph(count);
        reloadMapImage();
        ui->lblStatus->setText(QString("Сгенерирован граф: %1 вершин").arg(count));
    }
}

void MainWindow::on_btnAddEdge_clicked()
{
    if (!manager) return;
    currentState = AppState::SELECTING_EDGE_1;
    ui->lblStatus->setText("Добавление связи: выберите первую вершину");
}

void MainWindow::on_btnClearMap_clicked()
{
    if (!manager) {
        QMessageBox::information(this, "Инфо", "Карта еще не сгенерирована.");
        return;
    }

    manager->cleanMap();
    manager->drawAllCheckpoints();
    manager->saveMapToFile();

    reloadMapImage();

    ui->lblStatus->setText("Карта очищена (пути и вершины сброшены).");
}

void MainWindow::on_btnRemoveEdge_clicked()
{
    if (!manager) return;
    currentState = AppState::SELECTING_REMOVE_EDGE_1;
    ui->lblStatus->setText("Удаление связи: выберите первую вершину");
}

void MainWindow::on_btnFindPathNatural_clicked()
{
    if (!manager) return;
    if (!currentHero) {
        QMessageBox::warning(this, "Герой", "Сначала выберите героя из списка!");
        return;
    }
    isCastMode = false;
    currentState = AppState::SELECTING_PATH_1;
    ui->lblStatus->setText("Поиск (Natural): выберите старт");
}

void MainWindow::resetState()
{
    currentState = AppState::IDLE;
}

void MainWindow::on_comboHero_currentIndexChanged(int index)
{
    if (currentHero) {
        delete currentHero;
        currentHero = nullptr;
    }

    switch (index) {
        case 0: currentHero = nullptr; break;
        case 1: currentHero = new Human(); break;
        case 2: currentHero = new WoodElf(); break;
        case 3: currentHero = new Orc(); break;
        case 4: currentHero = new Gnome(); break;
        default: currentHero = nullptr;
    }

    if (currentHero) {
        QString heroName = QString::fromStdString(currentHero->getHeroName());
        ui->lblStatus->setText("Герой выбран: " + heroName);
    } else {
        ui->lblStatus->setText("Герой не выбран");
    }
}

void MainWindow::setupLegend()
{
    struct BiomeData {
        QString name;
        QColor color;
    };

    std::vector<BiomeData> biomes = {
            {"Равнина",               QColor(135, 171, 104)},
            {"Лес",                   QColor(20, 120, 40)},
            {"Морозная Равнина",      QColor(104, 171, 144)},
            {"Зимний лес",            QColor(18, 127, 91)},
            {"Ветренные Холмы",       QColor(102, 130, 79)},
            {"Лесистые Холмы",        QColor(8, 73, 22)},
            {"Холодные Холмы",        QColor(72, 116, 98)},
            {"Зимний Бор На Холмах",  QColor(9, 65, 46)},
            {"Каменные Скалы",        QColor(80, 80, 80)},
            {"Снежные Скалы",         QColor(195, 196, 197)}
    };

    QVBoxLayout* layout = ui->legendLayoutContainer;
    if (!layout) return;

    for (const auto& biome : biomes) {
        QWidget* rowWidget = new QWidget();
        QHBoxLayout* rowLayout = new QHBoxLayout(rowWidget);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        rowLayout->setSpacing(10);

        QLabel* colorBox = new QLabel();
        colorBox->setFixedSize(24, 24);
        QString style = QString("background-color: rgb(%1, %2, %3); border: 1px solid #555;")
                                .arg(biome.color.red())
                                .arg(biome.color.green())
                                .arg(biome.color.blue());
        colorBox->setStyleSheet(style);

        QLabel* textLabel = new QLabel(biome.name);
        textLabel->setStyleSheet("font-size: 12px;");
        textLabel->setWordWrap(true);

        rowLayout->addWidget(colorBox);
        rowLayout->addWidget(textLabel);

        layout->insertWidget(layout->count() - 1, rowWidget);
    }
}
