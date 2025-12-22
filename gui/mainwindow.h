#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem> // <--- Добавили
#include <QMouseEvent>
#include <vector>
#include <memory>

#include "PathfinderManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum class AppState {
    IDLE,
    SELECTING_EDGE_1,
    SELECTING_EDGE_2,
    SELECTING_PATH_1,
    SELECTING_PATH_2,
    SELECTING_REMOVE_EDGE_1,
    SELECTING_REMOVE_EDGE_2
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool eventFilter(QObject *watched, QEvent *event) override;

    // Переопределяем событие изменения размера окна, чтобы карта подстраивалась
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_btnGenerate_clicked();
    void on_btnAddEdge_clicked();
    void on_btnFindPath_clicked();
    void on_btnRemoveEdge_clicked();
    void on_comboHero_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *mapPixmapItem;

    // Квадратик, который показывает, какую клетку мы выбрали
    QGraphicsRectItem *selectionRect;

    PathfinderManager *manager;
    Hero *currentHero;

    int userGridWidth;
    int userGridHeight;
    double cellStepX;
    double cellStepY;
    const std::string mapFileName = "map.ppm";

    AppState currentState;
    QPoint tempPoint1;

    struct VisualEdge {
        int u_x, u_y, v_x, v_y;
    };
    std::vector<VisualEdge> visualEdges;

    void reloadMapImage();
    void drawOverlayGrid(int imgW, int imgH);
    void drawVisualEdges();
    void showContextMenu(const QPoint &screenPos, int gridX, int gridY);
    void resetState();

    // Вспомогательная функция для подсветки клетки
    void highlightCell(int gridX, int gridY);
};

#endif // MAINWINDOW_H
