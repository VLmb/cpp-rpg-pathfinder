#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
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
    SELECTING_REMOVE_EDGE_1, // Выбор первой вершины для удаления
    SELECTING_REMOVE_EDGE_2, // Выбор второй вершины для удаления
    SELECTING_PATH_1,
    SELECTING_PATH_2
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool eventFilter(QObject *watched, QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_btnGenerateNatural_clicked();
    void on_btnGenerateCast_clicked();
    void on_btnAddEdge_clicked();
    void on_btnClearMap_clicked();
    void on_btnRemoveEdge_clicked();
    void on_btnFindPathNatural_clicked();
    void on_btnFindPathCast_clicked();
    void on_comboHero_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *mapPixmapItem;
    QGraphicsRectItem *selectionRect;

    PathfinderManager *manager;
    Hero *currentHero;

    bool isCastMode;

    int userGridWidth;
    int userGridHeight;
    double cellStepX;
    double cellStepY;
    const std::string mapFileName = "map.ppm";

    AppState currentState;
    QPoint tempPoint1; // Хранит координаты первого клика

           // Вспомогательные методы
    void reloadMapImage();
    void drawOverlayGrid(int imgW, int imgH);
    void drawVisualEdges(); // Теперь берет данные из менеджера
    void showContextMenu(const QPoint &screenPos, int gridX, int gridY);
    void resetState();

    void runGenerationDialog(bool isCast);

    void setupLegend();

    void highlightCell(int gridX, int gridY);
};

#endif // MAINWINDOW_H
