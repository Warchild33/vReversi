#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gametree.h"
#include "gamewindow.h"
#include "gtreerenderer.h"
#include <QTransform>
#include <QMouseEvent>

QGraphicsScene* scene;
GameTree* game_tree;
GameWindow* gamewindow;
GTreeRenderer* renderer;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    //TreeTest();
    scene = new QGraphicsScene();
    int w = ui->graphicsView->width();
    scene->setSceneRect(0,0,3000,499);
    ui->graphicsView->setScene(scene);
    show();

    gamewindow = new GameWindow(this);
    renderer = new GTreeRenderer(scene, gamewindow->tree);
    game_tree = gamewindow->tree;
    connect(gamewindow->tree, SIGNAL(redrawSig()), renderer, SLOT(rebuild()));
    connect(gamewindow->tree, SIGNAL(redrawSig()), this, SLOT(rebuildTree()));
    connect(gamewindow, SIGNAL(newMove()), renderer, SLOT(rebuild()));
    connect(gamewindow, SIGNAL(newMove()), this, SLOT(rebuildTree()));

    ui->tabWidget->insertTab(0,  gamewindow, "game window");
    ui->tabWidget->setCurrentIndex(0);

}

void MainWindow::rebuildTree()
{

    QTreeWidgetItem* root_item = new QTreeWidgetItem(gamewindow->getTreeView());
    GTreeNode* node = game_tree->root;
    game_tree->SetLevels(node);
    root_item->setData(0,Qt::DisplayRole, QVariant(node->id));
    RenderTreeView(node, root_item);
}

void MainWindow::RenderTreeView(GTreeNode* node, QTreeWidgetItem* item )
{

    for(auto i=node->children.begin(); i!=node->children.end(); i++)
    {
        QTreeWidgetItem* child_item = new QTreeWidgetItem(item);
        child_item->setData(0,Qt::DisplayRole, QVariant((*i)->id));
        child_item->setData(1,Qt::DisplayRole, QVariant((*i)->value));
        child_item->setData(2,Qt::DisplayRole, QVariant((*i)->level));
        item->addChild(child_item);
        RenderTreeView((*i),child_item);
    }


}

void MainWindow::TreeTest()
{
    game_tree = new GameTree();
    game_tree->CreateTestTree();
    renderer = new GTreeRenderer(scene, game_tree);
    connect(game_tree, SIGNAL(redrawSig()), renderer, SLOT(rebuild()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if( event->button() == Qt::LeftButton )
    {
        x0 = event->pos().x() - ui->graphicsView->geometry().left();
        y0 = event->pos().y() - ui->graphicsView->geometry().top();
        QPointF p = ui->graphicsView->mapToScene(x0,y0);
        GTreeNode* node = game_tree->mouseHit(game_tree->root, p.x(), p.y());
        if(node)
        {
            node->game_state;
            ui->label->setText(QString::number(node->level));
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{


}

void MainWindow::on_scaleSlider_valueChanged(int value)
{
    QTransform t;
    t.reset();
    t.scale((float)value/1000,(float)value/1000);
    ui->graphicsView->setTransform(t);
}

void MainWindow::on_startMinimaxButton_clicked()
{

    game_tree->SetSlowMode();
    game_tree->StartMiniMax();

}

void MainWindow::on_resetButton_clicked()
{
    game_tree->Reset();
    renderer->RenderTree(game_tree->root);
}

void MainWindow::on_dy_slider_valueChanged(int value)
{
    int dx = ui->dx_slider->value();
    int dy = ui->dy_slider->value();

    QTransform t;
    t.reset();
    t = ui->graphicsView->transform();
    t.translate(dx,dy);
    ui->graphicsView->setTransform(t);
}

void MainWindow::on_dx_slider_valueChanged(int value)
{
    int dx = ui->dx_slider->value();
    int dy = ui->dy_slider->value();

    QTransform t;
    t.reset();
    t = ui->graphicsView->transform();
    t.translate(dx,dy);
    ui->graphicsView->setTransform(t);
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
}
