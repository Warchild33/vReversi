#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    cell_size = 10; //pixels
    n_move = 0;
    MAX_TREE_DEPTH = 5;
    current_move == MoveType::PLAYER;
    label_text = ui->label->text();
    //создать дерево ходов
    tree = new GameTree();
    tree->root = new GTreeNode( MAXER );
    SetScoreImage(0,0);
    showChoices();
    connect(this, SIGNAL(rebuildTree()), parent, SLOT(rebuildTree()));
}

GameWindow::~GameWindow()
{
    delete ui;
}

QTreeWidget* GameWindow::getTreeView()
{
    return ui->treeWidget;
}

void GameWindow::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    //draw vertical lines
    cell_size = ui->frame->width() / board.getN();
    int x = ui->frame->geometry().left();
    painter.setPen(Qt::black);
    for(int j=0; j < board.getN(); j++)
    {
        painter.drawLine(QPointF(x,ui->frame->geometry().top()), QPointF(x,ui->frame->geometry().bottom()));
        x+=cell_size;

    }

    //draw horizontal lines
    int y = ui->frame->geometry().top();
    for(int j=0; j < board.getN(); j++)
    {
        painter.drawLine(QPointF(ui->frame->geometry().left(),y), QPointF(ui->frame->geometry().right(),y));
        y+=cell_size;

    }

    //draw cells
    drawCells(painter);

}

void GameWindow::drawCells(QPainter& painter)
{
    int x0 = ui->frame->geometry().left();
    int y0 = ui->frame->geometry().top();

    for(int i=0; i < board.getN(); i++)
    for(int j=0; j < board.getN(); j++)
    {
        Cell* cell = board.getCell(i, j);
        switch( cell->state )
        {
            case CellState::NIL:
            break;
            case CellState::BLACK:
            {
              QPainterPath path;
              path.addEllipse(i*cell_size+x0, j*cell_size+y0, cell_size, cell_size);
              painter.fillPath(path, Qt::black);
              painter.drawPath(path);
            }
            break;
            case CellState::WHITE:
            {
              QPainterPath path2;
              path2.addEllipse(i*cell_size+x0, j*cell_size+y0, cell_size, cell_size);
              painter.fillPath(path2, Qt::white);
              painter.drawPath(path2);
            }
            break;
            case CellState::MARKED:
            {
              QPainterPath path3;
              path3.addEllipse(i*cell_size+x0+cell_size/4, j*cell_size+y0+cell_size/4, cell_size/2, cell_size/2);
              painter.fillPath(path3, Qt::green);
              painter.drawPath(path3);
            }
            break;
        }
    }


}

void GameWindow::SetScoreImage(int n_black, int n_white)
{
    QImage image(200,ui->label->height(),QImage::Format_ARGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    QPainterPath path;
    int size = 100/4;
    path.addEllipse(0,  0, size, size);
    painter.fillPath(path, Qt::black);
    QFont serifFont("Times", 15, QFont::Bold);
    painter.setFont(serifFont);
    painter.setPen(Qt::white);
    QRect rect1(0,  0, size, size);
    painter.drawText(rect1,Qt::AlignCenter,QString::number(n_black));
    painter.translate(size+5,0);
    painter.setPen(Qt::black);
    painter.drawText(rect1,Qt::AlignCenter,QString::number(n_white));
    painter.setPen(Qt::green);
    if( current_move == MoveType::COMPUTER )
        painter.drawPath(path);
    else
    {
        painter.translate(-size-5,0);
        painter.drawPath(path);
    }
    //image.save("image.png");
    ui->label_2->setPixmap(QPixmap::fromImage(image));
}

void GameWindow::makeMove(Cell* cell)
{
    if( current_move == MoveType::COMPUTER )
        cell->state = CellState::WHITE;
    else
        cell->state = CellState::BLACK;
    bool filled = board.fillLine(cell);
    if(filled)
        ui->label->setText(QString(label_text).arg("filled"));
    else
        ui->label->setText(QString(label_text).arg("not filled"));
    if(!filled)
    {
        cell->state = CellState::FREE;
        return;
    }
    if( current_move == MoveType::COMPUTER )
        current_move = MoveType::PLAYER;
    else
        current_move = MoveType::COMPUTER;
    n_move++;
}

std::vector<Cell*> GameWindow::showChoices(bool bMarkChoices)
{
    //highlight choices
    std::vector<Cell*> choices = board.selectCellsWithNeibours();
    CellState new_state;
    if( current_move == MoveType::COMPUTER )
        new_state = CellState::WHITE;
    else
        new_state = CellState::BLACK;


    std::vector<Cell*> choices_marked;
    for(auto c=choices.begin(); c!=choices.end(); c++)
    {
        (*c)->state = new_state;
        bool filled = board.checkLine((*c));
        if( filled )
        {
            (*c)->state = CellState::MARKED;
            choices_marked.push_back((*c));
        }
        else
          (*c)->state = CellState::FREE;
    }

    if( bMarkChoices ) board.markCells(choices_marked);
    return choices_marked;
}



void GameWindow::createGameTree(GTreeNode* root, int depth, bool maximizingPlayer)
{
    if(depth==0) return;

    board.loadFromArray(root->game_state);
    root->value = board.getValue();
    board.ResetMarked();
    std::vector<Cell*> choices = showChoices(false);

    for(auto c=choices.begin(); c!=choices.end(); c++)
    {
        board.save();
        if( maximizingPlayer )
            (*c)->state = CellState::BLACK;
        else
            (*c)->state = CellState::WHITE;
        board.fillLine((*c));
        GTreeNode* node1;
        if( maximizingPlayer )
            node1 = new GTreeNode( MAXER );
        else
            node1 = new GTreeNode( MINER );
        node1->value  = board.getValue();
        board.saveToArray(node1->game_state);
        board.restore();
        root->AddChild(node1);
        createGameTree(node1, depth-1, !maximizingPlayer);
        board.loadFromArray(root->game_state);
    }

}

void GameWindow::makeComputerMove()
{
    tree->root = new GTreeNode( MAXER );
    board.saveToArray(tree->root->game_state);
    //tree->free_mem(tree->root);
    createGameTree(tree->root, MAX_TREE_DEPTH, false);
    tree->root->value = tree->alphabeta(tree->root,MAX_TREE_DEPTH,-INF,INF,true);
    makeMoveByTree(tree->root);
    board.ResetMarked();
    showChoices();
    int n_black, n_white;
    board.CountScore(n_black, n_white);
    SetScoreImage(n_black, n_white);
    repaint();

}

void GameWindow::mousePressEvent(QMouseEvent* event)
{
    int x0 = ui->frame->geometry().left();
    int y0 = ui->frame->geometry().top();
    int i = (event->pos().x() - x0) / cell_size;
    int j = (event->pos().y() - y0) / cell_size;
    Cell* cell = board.getCell(i, j);
    board.ResetMarked();
    if( cell->state!=CellState::FREE ) return;
    //return;

    makeMove(cell);
    QTimer::singleShot(1000,this,SLOT(makeComputerMove()));

    int n_black, n_white;
    board.CountScore(n_black, n_white);
    SetScoreImage(n_black, n_white);

    //ui->label->setText(QString(label_text).arg(n_black).arg(n_white));
    repaint();
    emit newMove();
}

void GameWindow::makeMoveByTree(GTreeNode* root)
{
    GTreeNode* maxValueNode=root;
    int max=-1000000;
    for(auto i=root->children.begin(); i!=root->children.end(); i++)
    {
        if( (*i)->value > max )
        {
            maxValueNode = (*i);
            max = (*i)->value;
        }
    }
    board.loadFromArray(maxValueNode->game_state);
    if( current_move == MoveType::COMPUTER )
        current_move = MoveType::PLAYER;
    else
        current_move = MoveType::COMPUTER;
    n_move++;
}

void GameWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    GTreeNode* node = tree->getNodeByID(item->data(0,Qt::DisplayRole).toInt(),
                                             tree->root,tree->root);
    if(node)
    {
        board.loadFromArray(node->game_state);
    }
    repaint();
}

void GameWindow::on_minimaxButton_clicked()
{
    tree->root->value = tree->alphabeta(tree->root,9,-INF,INF,true);
    //tree->root->value = tree->minimax(tree->root,MAX_TREE_DEPTH,true);
    emit rebuildTree();

}
