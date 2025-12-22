#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <vector>
#include <memory>

// Подключаем ваши заголовочные файлы из Core
// Если IDE их не видит, проверьте target_include_directories в CMake
#include "PathfinderManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Состояния интерфейса для конечного автомата кликов
enum class AppState {
    IDLE,               // Обычный режим
    SELECTING_EDGE_1,   // Ждем первую точку для связи
    SELECTING_EDGE_2,   // Ждем вторую точку для связи
    SELECTING_PATH_1,   // Ждем старт пути
    SELECTING_PATH_2    // Ждем финиш пути
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

           // Перехват событий мыши для QGraphicsView
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_btnGenerate_clicked();
    void on_btnAddEdge_clicked();
    void on_btnFindPath_clicked();
    void on_comboHero_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

           // Графическая сцена
    QGraphicsScene *scene;
    QGraphicsPixmapItem *mapPixmapItem;

           // Логика приложения (Core)
    PathfinderManager *manager;
    Hero *currentHero;

           // Размеры сетки (в клетках), заданные пользователем
    int userGridWidth;
    int userGridHeight;

           // Вычисленные размеры одной клетки в пикселях (для текущего изображения)
    double cellStepX;
    double cellStepY;

           // Путь к файлу карты
    const std::string mapFileName = "map.ppm";

           // Текущее состояние
    AppState currentState;
    QPoint tempPoint1; // Хранит координаты первого клика

           // Структура для запоминания нарисованных связей (чтобы не пропадали при обновлении)
    struct VisualEdge {
        int u_x, u_y, v_x, v_y;
    };
    std::vector<VisualEdge> visualEdges;

           // Вспомогательные методы
    void reloadMapImage();
    void drawOverlayGrid(int imgW, int imgH);
    void drawVisualEdges();
    void showContextMenu(const QPoint &screenPos, int gridX, int gridY);
    void resetState();
};

#endif // MAINWINDOW_H
