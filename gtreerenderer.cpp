#include <QDebug>
#include <QGraphicsTextItem>
#include "gtreerenderer.h"

GTreeRenderer::GTreeRenderer(QGraphicsScene* scene, GameTree* tree)
{
    my_scene = scene;
    my_tree = tree;

    wr = 100;
    hr = 40;
    gap = 30;
    C = QPointF(scene->width()/2,hr);
    QRectF rect;
    rect.setX(C.x()-wr/2);
    rect.setY(C.y()-hr/2);
    rect.setWidth(wr);
    rect.setHeight(hr);
    my_scene->addRect(rect);
    //QGraphicsTextItem* text_label = my_scene->addText("0:1:1");
    //text_label->setPos(C.x()-text_label->boundingRect().width()/2,C.y()-text_label->boundingRect().height()/2);

    tree->root->setRect(rect);
    RenderTree(tree->root);


}

QString set_val(int v)
{
    if(v == INF) return "INF";
    if(v == -INF) return "-INF";
    return QString::number(v);
}

void GTreeRenderer::RenderTree(GTreeNode* node)
{
    //получаем центр прямоугольника ноды
    C.setX(node->rect.x() + node->rect.width()/2);
    C.setY(node->rect.y() + node->rect.height()/2);

    //получаем точку D сдвигом вниз
    D.setX(C.x());
    D.setY(C.y()+4*hr);

    //промежуток зависит от количества потомков
    switch(node->level)
    {
       case 0: gap = 500; break;
       case 1: gap = 200; break;
       case 2: gap = 50; break;
    }

    //если нечетное число потомков
    if((node->children.size()%2)==1)
    {
        //отрисовываем один квадрат сразу
        int n_child = node->children.size()/2;
        QRectF child_rc(D.x()-wr/2,D.y()-hr/2,wr,hr);
        node->children[n_child]->setRect(child_rc);
        my_scene->addRect(child_rc);

        // идем вправо
        int x = D.x() + wr/2 + gap;
        for(int i=n_child+1; i < node->children.size(); i++ )
        {
            QRectF child_rc(x,D.y()-hr/2,wr,hr);
            node->children[i]->setRect(child_rc);
            my_scene->addRect(child_rc);
            x+=wr+gap;
        }

        //идем влево
        x = D.x() - wr/2 - gap;
        for(int i=n_child-1; i >= 0; i-- )
        {
            QRectF child_rc(x-wr,D.y()-hr/2,wr,hr);
            node->children[i]->setRect(child_rc);
            my_scene->addRect(child_rc);
            x-=wr+gap;
        }

    }
    else
    {
        // идем вправо
        int x = D.x() + gap/2;
        for(int i=node->children.size()/2; i < node->children.size(); i++ )
        {
            QRectF child_rc(x,D.y()-hr/2,wr,hr);
            node->children[i]->setRect(child_rc);
            my_scene->addRect(child_rc);
            x+=wr+gap;
        }

        //идем влево
        x = D.x() - gap/2;
        for(int i=node->children.size()/2-1; i >= 0; i-- )
        {
            QRectF child_rc(x-wr,D.y()-hr/2,wr,hr);
            node->children[i]->setRect(child_rc);
            my_scene->addRect(child_rc);
            x-=wr+gap;
        }
    }

    QString v,a,b;
    v = set_val(node->value);
    a = set_val(node->alpha);
    b = set_val(node->beta);
    QGraphicsTextItem* text_label = my_scene->addText(v + ":" + a + ":" + b);
    text_label->setPos(C.x()-text_label->boundingRect().width()/2,C.y()-text_label->boundingRect().height()/2);

    for(int i=0; i < node->children.size(); i++)
    {
       my_scene->addLine(node->rect.center().x(),node->rect.center().y()+hr/2,
                         node->children[i]->rect.center().x(),node->children[i]->rect.center().y()-hr/2);
       RenderTree(node->children[i]);
    }


}



void GTreeRenderer::rebuild()
{
    my_scene->clear();
    RenderTree(my_tree->root);
    qDebug() << "redraw";
}
