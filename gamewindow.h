#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QTreeWidget>
#include <QDialog>
#include <vector>
#include <map>
#include "board.h"
#include "gametree.h"

namespace Ui {
class GameWindow;
}


class GameWindow : public QDialog
{
    Q_OBJECT

public:
    MoveType current_move;
    int      n_move;
    Board   board;
    int     cell_size;
    QString label_text;
    int     MAX_TREE_DEPTH;
    GameTree* tree;
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void drawCells(QPainter& painter);
    void SetScoreImage(int n_black, int n_white);
    void makeMove(Cell* cell);
    QTreeWidget* getTreeView();
    std::vector<Cell*> showChoices(bool bMarkChoices=true);
    void createGameTree(GTreeNode* node, int depth, bool maximizingPlayer);
    void makeMoveByTree(GTreeNode* root);
signals:
    void newMove();
    void rebuildTree();

public:
    Ui::GameWindow *ui;
private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_minimaxButton_clicked();
    void makeComputerMove();
};

#endif // GAMEWINDOW_H
