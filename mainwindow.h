#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include "gametree.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int x0,y0;

private slots:
    void TreeTest();
    void on_scaleSlider_valueChanged(int value);
    void on_startMinimaxButton_clicked();
    void on_resetButton_clicked();
    void rebuildTree();
    void RenderTreeView(GTreeNode* node, QTreeWidgetItem* item );
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

    void on_dy_slider_valueChanged(int value);

    void on_dx_slider_valueChanged(int value);

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
