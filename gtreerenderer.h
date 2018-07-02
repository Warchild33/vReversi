#ifndef GTREERENDERER_H
#define GTREERENDERER_H

#include "gametree.h"
#include <QGraphicsScene>
#include <QObject>
#include <QTreeWidget>

class GTreeRenderer : public QObject
{
    Q_OBJECT
public:
    QPointF C,D;
    int wr,hr,gap;
    QGraphicsScene* my_scene;
    GameTree*       my_tree;
    GTreeRenderer(QGraphicsScene* scene, GameTree* tree);
    void RenderTree(GTreeNode* node);
    void RenderTreeView(GTreeNode* node, QTreeWidgetItem* item );
public slots:
    void rebuild();
};

#endif // GTREERENDERER_H
