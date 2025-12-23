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
      , isCastMode(false) // Инициализация флага
{
    ui->setupUi(this);

           // Настройка сцены
    ui->mapView->setScene(scene);
    ui->mapView->viewport()->installEventFilter(this);

           // Настройки зума и скролла
    ui->mapView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->mapView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    ui->mapView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mapView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

           // === ЗАПОЛНЕНИЕ СПИСКА ГЕРОЕВ ИЗ КОДА ===
    ui->comboHero->clear(); // Удаляем то, что было в .ui файле
    ui->comboHero->addItem(""); // Индекс 0 - Пустой выбор

           // Создаем временные объекты, чтобы узнать их имена
    Human tempHuman;
    ui->comboHero->addItem(QString::fromStdString(tempHuman.getHeroName())); // Индекс 1

    WoodElf tempElf;
    ui->comboHero->addItem(QString::fromStdString(tempElf.getHeroName()));   // Индекс 2

    Orc tempOrc;
    ui->comboHero->addItem(QString::fromStdString(tempOrc.getHeroName()));   // Индекс 3

    Gnome tempGnome;
    ui->comboHero->addItem(QString::fromStdString(tempGnome.getHeroName())); // Индекс 4

           // Выбираем пустой элемент по умолчанию
    ui->comboHero->setCurrentIndex(0);

    ui->lblStatus->setText("Готово к работе. Сгенерируйте карту.");
}

MainWindow::~MainWindow()
{
    delete ui;
    if (manager) delete manager;
    if (currentHero) delete currentHero;
}

// === Отрисовка и загрузка ===

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

// === События ===

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->mapView->viewport()) {

        // Зум
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

               // Клики
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
                        showContextMenu(me->globalPos(), gridX, gridY);
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

                               // Передаем флаг isCastMode последним параметром
                        double time = manager->findPathAndDraw(
                                tempPoint1.x(), tempPoint1.y(),
                                gridX, gridY,
                                *currentHero,
                                isCastMode
                                );

                        if (std::isinf(time)) {
                            QMessageBox::warning(this, "Нет пути", "Не существует пути между этими вершинами!");
                            ui->lblStatus->setText("Путь не найден (бесконечность).");
                        } else {
                            // Получаем имя героя и режим
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

// === Кнопки ===

void MainWindow::on_btnGenerate_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Параметры");
    QFormLayout form(&dialog);

    QSpinBox *spinW = new QSpinBox(&dialog); spinW->setRange(2, 2000); spinW->setValue(30);
    QSpinBox *spinH = new QSpinBox(&dialog); spinH->setRange(2, 2000); spinH->setValue(20);

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

        manager = new PathfinderManager(userGridWidth, userGridHeight, mapFileName, scaleVal, spinSeed->value());
        manager->saveMapToFile();

        reloadMapImage();

        ui->lblStatus->setText("Карта сгенерирована.");
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

           // Вызываем метод очистки в менеджере
    manager->cleanMap();

    manager->drawAllCheckpoints();
    manager->saveMapToFile();

           // Перерисовываем картинку
    reloadMapImage();

    ui->lblStatus->setText("Карта очищена (пути и вершины сброшены).");
}

void MainWindow::on_btnRemoveEdge_clicked()
{
    if (!manager) return;
    currentState = AppState::SELECTING_REMOVE_EDGE_1;
    ui->lblStatus->setText("Удаление связи: выберите первую вершину");
}

// === НОВЫЕ СЛОТЫ ДЛЯ ПОИСКА ПУТИ ===

void MainWindow::on_btnFindPathNatural_clicked()
{
    if (!manager) return;
    if (!currentHero) {
        QMessageBox::warning(this, "Герой", "Сначала выберите героя из списка!");
        return;
    }
    isCastMode = false; // Режим Natural
    currentState = AppState::SELECTING_PATH_1;
    ui->lblStatus->setText("Поиск (Natural): выберите старт");
}

void MainWindow::on_btnFindPathCast_clicked()
{
    if (!manager) return;
    if (!currentHero) {
        QMessageBox::warning(this, "Герой", "Сначала выберите героя из списка!");
        return;
    }
    isCastMode = true; // Режим Cast
    currentState = AppState::SELECTING_PATH_1;
    ui->lblStatus->setText("Поиск (Cast): выберите старт");
}

// ===================================

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

           // Индекс 0 = "", Индексы 1..4 = Герои
    switch (index) {
        case 0: currentHero = nullptr; break;
        case 1: currentHero = new Human(); break;
        case 2: currentHero = new WoodElf(); break;
        case 3: currentHero = new Orc(); break;
        case 4: currentHero = new Gnome(); break;
        default: currentHero = nullptr;
    }

    if (currentHero) {
        // Берем имя уже из созданного объекта
        QString heroName = QString::fromStdString(currentHero->getHeroName());
        ui->lblStatus->setText("Герой выбран: " + heroName);
    } else {
        ui->lblStatus->setText("Герой не выбран");
    }
}
