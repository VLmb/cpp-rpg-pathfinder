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
{
    ui->setupUi(this);

           // Настройка сцены
    ui->mapView->setScene(scene);
    ui->mapView->viewport()->installEventFilter(this);

    // Настройки для зума и скролла
    ui->mapView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->mapView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    ui->mapView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->mapView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

           // === ИЗМЕНЕНИЕ: Настройка списка героев ===
           // Добавляем пустой пункт в начало, чтобы заставить пользователя выбрать
    ui->comboHero->insertItem(0, "");
    ui->comboHero->setCurrentIndex(0); // Выбираем пустой пункт по умолчанию

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
    // Если файла еще нет (до генерации), просто выходим
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

    // Растягиваем на всё окно
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
    QPen edgePen(Qt::red);
    edgePen.setWidth(1);

    for (const auto &edge : visualEdges) {
        double x1 = (edge.u_x * cellStepX) + (cellStepX / 2.0);
        double y1 = (edge.u_y * cellStepY) + (cellStepY / 2.0);
        double x2 = (edge.v_x * cellStepX) + (cellStepX / 2.0);
        double y2 = (edge.v_y * cellStepY) + (cellStepY / 2.0);

        QGraphicsLineItem *line = scene->addLine(x1, y1, x2, y2, edgePen);
        line->setZValue(2);
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

void MainWindow::on_btnRemoveEdge_clicked()
{
    if (!manager) return;
    currentState = AppState::SELECTING_REMOVE_EDGE_1;
    ui->lblStatus->setText("Удаление связи: выберите первую вершину");
}

// === События ===

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->mapView->viewport()) {

        // Зум колесиком
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
                        visualEdges.push_back({tempPoint1.x(), tempPoint1.y(), gridX, gridY});
                        ui->lblStatus->setText("Связь добавлена.");
                        reloadMapImage();
                    } else {
                        ui->lblStatus->setText("Ошибка: Нельзя создать связь (проверьте вершины).");
                    }
                    resetState();
                    break;
                case AppState::SELECTING_REMOVE_EDGE_1:
                tempPoint1 = QPoint(gridX, gridY);
                currentState = AppState::SELECTING_REMOVE_EDGE_2;
                ui->lblStatus->setText(QString("Удаление: [%1, %2]. Выберите вторую вершину.").arg(gridX).arg(gridY));
                break;

            case AppState::SELECTING_REMOVE_EDGE_2:
                // 1. Пытаемся удалить в логике (Manager)
                if (manager->removeEdgeFromCheckpointGraph(tempPoint1.x(), tempPoint1.y(), gridX, gridY)) {

                    // 2. Если успех — удаляем визуальную линию из списка
                    // Нам нужно найти ребро (u,v) или (v,u), так как граф неориентированный
                    auto it = std::remove_if(visualEdges.begin(), visualEdges.end(),
                        [&](const VisualEdge& edge) {
                            bool matchDirect = (edge.u_x == tempPoint1.x() && edge.u_y == tempPoint1.y() &&
                                                edge.v_x == gridX && edge.v_y == gridY);
                            bool matchReverse = (edge.u_x == gridX && edge.u_y == gridY &&
                                                 edge.v_x == tempPoint1.x() && edge.v_y == tempPoint1.y());
                            return matchDirect || matchReverse;
                        });

                    // Фактически удаляем элементы из вектора
                    if (it != visualEdges.end()) {
                        visualEdges.erase(it, visualEdges.end());
                    }

                    ui->lblStatus->setText("Связь удалена.");
                    reloadMapImage(); // Перерисовка (линия исчезнет)
                } else {
                    // 3. Если ошибка (связи не было или вершины не существуют)
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

                        // === ИЗМЕНЕНИЕ: Убрали mapFileName из вызова ===
                        double time = manager->findPathAndDraw(tempPoint1.x(), tempPoint1.y(), gridX, gridY, *currentHero);

                        ui->lblStatus->setText(QString("Время пути: %1").arg(time));
                        reloadMapImage();
                    } catch (const std::exception &e) {
                        ui->lblStatus->setText(QString("Ошибка: %1").arg(e.what()));
                        QMessageBox::warning(this, "Ошибка", e.what());
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
    dialog.setWindowTitle("Параметры карты");
    QFormLayout form(&dialog);

    QSpinBox *spinW = new QSpinBox(&dialog); spinW->setRange(2, 2000); spinW->setValue(20);
    QSpinBox *spinH = new QSpinBox(&dialog); spinH->setRange(2, 2000); spinH->setValue(20);

    QDoubleSpinBox *spinScale = new QDoubleSpinBox(&dialog);
    spinScale->setRange(0.0, 500.0);
    spinScale->setSingleStep(0.1);
    spinScale->setValue(0.0f); // Default 0.0

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

               // === ИЗМЕНЕНИЕ: mapFileName передаем в конструктор ===
        manager = new PathfinderManager(userGridWidth, userGridHeight, mapFileName, scaleVal, spinSeed->value());

        // === ИЗМЕНЕНИЕ: Убрали mapFileName из вызова, если метод больше не принимает аргументов ===
        manager->saveMapToFile();

        visualEdges.clear();
        reloadMapImage();

        ui->lblStatus->setText("Карта сгенерирована.");
    }
}

void MainWindow::on_btnAddEdge_clicked()
{
    if (!manager) return;
    currentState = AppState::SELECTING_EDGE_1;
    ui->lblStatus->setText("Связь: выберите первую клетку (Left Click)");
}

void MainWindow::on_btnFindPath_clicked()
{
    if (!manager) return;
    // Проверка на nullptr (герой должен быть выбран)
    if (!currentHero) {
        QMessageBox::warning(this, "Герой", "Сначала выберите героя из списка!");
        return;
    }
    currentState = AppState::SELECTING_PATH_1;
    ui->lblStatus->setText("Поиск пути: выберите старт (Left Click)");
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

    // === ИЗМЕНЕНИЕ: Сдвиг индексов из-за пустой первой строки ===
    // 0 = "" (пусто)
    // 1 = Human
    // 2 = Wood Elf
    // 3 = Orc
    // 4 = Gnome

    switch (index) {
        case 0:
            currentHero = nullptr;
            break;
        case 1: currentHero = new Human(); break;
        case 2: currentHero = new WoodElf(); break;
        case 3: currentHero = new Orc(); break;
        case 4: currentHero = new Gnome(); break;
        default: currentHero = nullptr;
    }

    if (currentHero) {
        ui->lblStatus->setText("Герой выбран: " + ui->comboHero->currentText());
    } else {
        ui->lblStatus->setText("Герой не выбран");
    }
}
