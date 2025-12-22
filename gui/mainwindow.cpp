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
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
      , scene(new QGraphicsScene(this))
      , mapPixmapItem(nullptr)
      , manager(nullptr)
      , currentHero(nullptr)
      , userGridWidth(0)
      , userGridHeight(0)
      , cellStepX(1.0)
      , cellStepY(1.0)
      , currentState(AppState::IDLE)
{
    ui->setupUi(this);

           // Настраиваем сцену
    ui->mapView->setScene(scene);

    // Включаем отслеживание мыши
    ui->mapView->viewport()->installEventFilter(this);

    // Сглаживание для красивых линий сетки
    ui->mapView->setRenderHint(QPainter::Antialiasing);

    ui->lblStatus->setText("Добро пожаловать. Сгенерируйте карту.");
}

MainWindow::~MainWindow()
{
    delete ui;
    if (manager) delete manager;
    if (currentHero) delete currentHero;
}

// =========================================================
//                  ЛОГИКА ОТРИСОВКИ
// =========================================================

void MainWindow::reloadMapImage()
{
    qDebug() << "Текущая рабочая папка:" << QDir::currentPath();
    qDebug() << "Ищу файл здесь:" << QDir::current().absoluteFilePath(QString::fromStdString(mapFileName));

    if (!QFile::exists(QString::fromStdString(mapFileName))) {
        qDebug() << "ОШИБКА: Файл не найден!";
        ui->lblStatus->setText("Файл map.ppm не найден! Сгенерируйте карту.");
        return; // Выходим, чтобы не крашнулось
    }
    // ======================================

    scene->clear();
    // 1. Очищаем сцену
    scene->clear();
    mapPixmapItem = nullptr;

           // 2. Загружаем изображение принудительно (QImage обходит кэш QPixmap)
    QImage image;
    if (!image.load(QString::fromStdString(mapFileName))) {
        ui->lblStatus->setText("Ошибка: файл карты не найден (map.ppm).");
        return;
    }

    int imgW = image.width();
    int imgH = image.height();

           // 3. Добавляем карту на сцену
    mapPixmapItem = scene->addPixmap(QPixmap::fromImage(image));
    mapPixmapItem->setZValue(0); // Слой 0 (фон)

           // 4. Рассчитываем шаг сетки
           // Если картинка 1000px, а сетка 10 клеток, шаг = 100px
    if (userGridWidth > 0 && userGridHeight > 0) {
        cellStepX = (double)imgW / userGridWidth;
        cellStepY = (double)imgH / userGridHeight;
    }

           // 5. Рисуем сетку и связи поверх
    drawOverlayGrid(imgW, imgH);
    drawVisualEdges();

           // 6. Подгоняем размер сцены
    scene->setSceneRect(0, 0, imgW, imgH);
}

void MainWindow::drawOverlayGrid(int imgW, int imgH)
{
    QPen gridPen(Qt::black);
    gridPen.setWidth(0); // Косметическое перо (всегда 1 пиксель)
    gridPen.setColor(QColor(0, 0, 0, 150)); // Полупрозрачный черный

           // Вертикальные линии
    for (int i = 0; i <= userGridWidth; ++i) {
        double x = i * cellStepX;
        QGraphicsLineItem *line = scene->addLine(x, 0, x, imgH, gridPen);
        line->setZValue(1); // Слой 1
    }

           // Горизонтальные линии
    for (int i = 0; i <= userGridHeight; ++i) {
        double y = i * cellStepY;
        QGraphicsLineItem *line = scene->addLine(0, y, imgW, y, gridPen);
        line->setZValue(1);
    }
}

void MainWindow::drawVisualEdges()
{
    QPen edgePen(Qt::red);
    edgePen.setWidth(2);

    for (const auto &edge : visualEdges) {
        // Вычисляем центры клеток
        double x1 = (edge.u_x * cellStepX) + (cellStepX / 2.0);
        double y1 = (edge.u_y * cellStepY) + (cellStepY / 2.0);
        double x2 = (edge.v_x * cellStepX) + (cellStepX / 2.0);
        double y2 = (edge.v_y * cellStepY) + (cellStepY / 2.0);

        QGraphicsLineItem *line = scene->addLine(x1, y1, x2, y2, edgePen);
        line->setZValue(2); // Слой 2 (самый верхний)
    }
}

// =========================================================
//                  СОБЫТИЯ МЫШИ
// =========================================================

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->mapView->viewport() && event->type() == QEvent::MouseButtonPress) {
        if (!manager) return false;

        QMouseEvent *me = static_cast<QMouseEvent*>(event);
        QPointF scenePos = ui->mapView->mapToScene(me->pos());

               // Переводим пиксели сцены в координаты сетки
        int gridX = static_cast<int>(scenePos.x() / cellStepX);
        int gridY = static_cast<int>(scenePos.y() / cellStepY);

               // Проверка границ
        if (gridX < 0 || gridX >= userGridWidth || gridY < 0 || gridY >= userGridHeight) {
            return false;
        }

        switch (currentState) {
            case AppState::IDLE:
                // Правый клик открывает меню
                if (me->button() == Qt::RightButton) {
                    showContextMenu(me->globalPos(), gridX, gridY); // globalPos для меню!
                }
                break;

            case AppState::SELECTING_EDGE_1:
                tempPoint1 = QPoint(gridX, gridY);
                currentState = AppState::SELECTING_EDGE_2;
                ui->lblStatus->setText(QString("Связь: выбрано [%1, %2]. Выберите вторую вершину.").arg(gridX).arg(gridY));
                break;

            case AppState::SELECTING_EDGE_2:
                // Пытаемся добавить ребро
                if (manager->addEdgeToCheckpointGraph(tempPoint1.x(), tempPoint1.y(), gridX, gridY)) {
                    // Если успех — сохраняем визуально и обновляем
                    visualEdges.push_back({tempPoint1.x(), tempPoint1.y(), gridX, gridY});
                    ui->lblStatus->setText("Связь успешно создана!");
                    reloadMapImage();
                } else {
                    QMessageBox::warning(this, "Ошибка", "Не удалось создать связь (проверьте, существуют ли вершины).");
                    ui->lblStatus->setText("Ошибка создания связи");
                }
                resetState();
                break;

            case AppState::SELECTING_PATH_1:
                tempPoint1 = QPoint(gridX, gridY);
                currentState = AppState::SELECTING_PATH_2;
                ui->lblStatus->setText(QString("Путь: старт [%1, %2]. Выберите финиш.").arg(gridX).arg(gridY));
                break;

            case AppState::SELECTING_PATH_2:
                try {
                    if (!currentHero) throw std::runtime_error("Герой не выбран!");

                           // Основной метод поиска
                    double time = manager->findPathAndDraw(tempPoint1.x(), tempPoint1.y(), gridX, gridY, *currentHero, mapFileName);

                    ui->lblStatus->setText(QString("Путь найден! Время: %1").arg(time));
                    // Обновляем картинку (там нарисуется путь)
                    reloadMapImage();

                } catch (const std::exception &e) {
                    QMessageBox::critical(this, "Ошибка поиска пути", e.what());
                    ui->lblStatus->setText("Ошибка поиска пути");
                }
                resetState();
                break;
        }
        return true;
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::showContextMenu(const QPoint &screenPos, int gridX, int gridY)
{
    QMenu menu;
    QAction *addAct = menu.addAction("Добавить вершину");
    QAction *delAct = menu.addAction("Удалить вершину");

    QAction *selected = menu.exec(screenPos);

    if (selected == addAct) {
        manager->addVertexToCheckpointGraph(gridX, gridY);
        ui->lblStatus->setText(QString("Добавлена вершина: %1, %2").arg(gridX).arg(gridY));
        reloadMapImage();
    }
    else if (selected == delAct) {
        if (manager->removeVertexFromCheckpointGraph(gridX, gridY)) {
            ui->lblStatus->setText(QString("Удалена вершина: %1, %2").arg(gridX).arg(gridY));
            // В идеале нужно удалить и визуальные ребра, связанные с этой точкой
            // Здесь для простоты просто перерисовываем
            reloadMapImage();
        } else {
            ui->lblStatus->setText("Не удалось удалить вершину");
        }
    }
}

// =========================================================
//                  КНОПКИ И МЕНЮ
// =========================================================

void MainWindow::on_btnGenerate_clicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Генерация карты");
    QFormLayout form(&dialog);

    QSpinBox *spinW = new QSpinBox(&dialog); spinW->setRange(2, 500); spinW->setValue(10);
    QSpinBox *spinH = new QSpinBox(&dialog); spinH->setRange(2, 500); spinH->setValue(10);
    QDoubleSpinBox *spinScale = new QDoubleSpinBox(&dialog); spinScale->setRange(0.1, 100.0); spinScale->setValue(1.0);
    QSpinBox *spinSeed = new QSpinBox(&dialog); spinSeed->setRange(0, 999999); spinSeed->setValue(12345);

    form.addRow("Ширина (клеток):", spinW);
    form.addRow("Высота (клеток):", spinH);
    form.addRow("Масштаб шума:", spinScale);
    form.addRow("Seed:", spinSeed);

    QDialogButtonBox btnBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&btnBox);

    connect(&btnBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&btnBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        if (manager) delete manager;

        userGridWidth = spinW->value();
        userGridHeight = spinH->value();

               // Создаем менеджер (он генерирует внутреннюю карту)
        manager = new PathfinderManager(userGridWidth, userGridHeight, spinScale->value(), spinSeed->value());

        // Сохраняем результат в файл
        manager->saveMapToFile(mapFileName);

        // Сбрасываем старые связи
        visualEdges.clear();

        // Загружаем и рисуем
        reloadMapImage();
        ui->lblStatus->setText("Карта сгенерирована.");
    }
}

void MainWindow::on_btnAddEdge_clicked()
{
    if (!manager) {
        QMessageBox::information(this, "Инфо", "Сначала сгенерируйте карту!");
        return;
    }
    currentState = AppState::SELECTING_EDGE_1;
    ui->lblStatus->setText("Выберите первую вершину для связи...");
}

void MainWindow::on_btnFindPath_clicked()
{
    if (!manager) {
        QMessageBox::information(this, "Инфо", "Сначала сгенерируйте карту!");
        return;
    }
    if (!currentHero) {
        QMessageBox::warning(this, "Герой", "Пожалуйста, выберите героя из списка!");
        return;
    }
    currentState = AppState::SELECTING_PATH_1;
    ui->lblStatus->setText("Выберите стартовую точку пути...");
}

void MainWindow::on_comboHero_currentIndexChanged(int index)
{
    if (currentHero) {
        delete currentHero;
        currentHero = nullptr;
    }

    // Внимание: Индексы должны совпадать с порядком в comboHero в UI
    switch (index) {
        case 0: currentHero = new Human(); break;
        case 1: currentHero = new WoodElf(); break;
        case 2: currentHero = new Orc(); break;
        case 3: currentHero = new Gnome(); break;
        default:
            ui->lblStatus->setText("Неизвестный тип героя");
            return;
    }

    ui->lblStatus->setText("Герой изменен: " + ui->comboHero->currentText());
}

void MainWindow::resetState()
{
    currentState = AppState::IDLE;
    // Сбрасываем текст статуса, если там висело приглашение к выбору
    if (ui->lblStatus->text().contains("Выберите")) {
        ui->lblStatus->setText("Готово");
    }
}
