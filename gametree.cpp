#include <QApplication>
#include <QDebug>
#include <math.h>
#include <algorithm>
#include "gametree.h"
#include <unistd.h>

GameTree::GameTree()
    :QObject()
{
    bSlowMode = false;
}

void GameTree::CreateTestTree()
{
    root = new GTreeNode( MAXER );

    // 1 level
    GTreeNode* node1 = new GTreeNode( MINER );
    GTreeNode* node2 = new GTreeNode( MINER );
    GTreeNode* node3 = new GTreeNode( MINER );

    //2 level
    GTreeNode* node4 = new GTreeNode( MAXER );
    GTreeNode* node5 = new GTreeNode( MAXER );
    GTreeNode* node6 = new GTreeNode( MAXER );
    GTreeNode* node7 = new GTreeNode( MAXER );
    GTreeNode* node8 = new GTreeNode( MAXER );
    GTreeNode* node9 = new GTreeNode( MAXER );

    //3 level
    GTreeNode* node11 = new GTreeNode( MINER, 4 );
    GTreeNode* node12 = new GTreeNode( MINER, 6 );
    GTreeNode* node13 = new GTreeNode( MINER, 7 );
    GTreeNode* node14 = new GTreeNode( MINER, 9 );
    GTreeNode* node15 = new GTreeNode( MINER, 1 );
    GTreeNode* node16 = new GTreeNode( MINER, 2 );
    GTreeNode* node17 = new GTreeNode( MINER, 0 );
    GTreeNode* node18 = new GTreeNode( MINER, 1 );
    GTreeNode* node19 = new GTreeNode( MINER, 8 );
    GTreeNode* node20 = new GTreeNode( MINER, 1 );
    GTreeNode* node21 = new GTreeNode( MINER, 9 );
    GTreeNode* node22 = new GTreeNode( MINER, 2 );

    //Connections
    root->AddChild(node1);
    root->AddChild(node2);
    root->AddChild(node3);
    node1->AddChild(node4);
    node1->AddChild(node5);
    node2->AddChild(node6);
    node2->AddChild(node7);
    node3->AddChild(node8);
    node3->AddChild(node9);
    node4->AddChild(node11);
    node4->AddChild(node12);
    node5->AddChild(node13);
    node5->AddChild(node14);
    node6->AddChild(node15);
    node6->AddChild(node16);
    node7->AddChild(node17);
    node7->AddChild(node18);
    node8->AddChild(node19);
    node8->AddChild(node20);
    node9->AddChild(node21);
    node9->AddChild(node22);

    //set Levels
    root->setLevel(0);
    SetLevels(root);

}

//Start MiniMax algorithm
void GameTree::StartMiniMax()
{
    //while( root->value == -INF || root->value == INF )
    //{
    //    MiniMax(root);
    //}
    root->value = alphabeta(root,3,-INF,INF,true);
    //root->value = minimax(root,3,true);
    emit redrawSig();
    qDebug() << "We done!";
}

int chooseMin(std::vector<GTreeNode*>& children)
{
    std::vector<GTreeNode*>::iterator it;
    int minVal=INF+1;
    for(it=children.begin(); it!=children.end(); it++)
    {
        if((*it)->value < minVal)
            minVal = (*it)->value;

    }
    return minVal;
}

int chooseMax(std::vector<GTreeNode*>& children)
{
    std::vector<GTreeNode*>::iterator it;
    int maxVal=-INF-1;
    for(it=children.begin(); it!=children.end(); it++)
    {
        if((*it)->value > maxVal)
            maxVal = (*it)->value;

    }
    return maxVal;
}

void GameTree::MakeReset(GTreeNode* node)
{
    std::vector<GTreeNode*>::iterator it;
    if(!node->isLeaf())
    {
        if( node->type == MAXER)
            node->value = -INF;
        if( node->type == MINER)
            node->value = INF;
    }
    for(it=node->children.begin(); it!=node->children.end(); it++)
       MakeReset(*it);
}

void GameTree::Reset()
{
    MakeReset(root);
}

GTreeNode* GameTree::mouseHit(GTreeNode* root, int x, int y)
{
    if( root->rect.contains(x,y) )
        return root;
    for(auto it=root->children.begin(); it!=root->children.end(); it++)
    {
        mouseHit((*it), x, y);
    }
    return 0;
}

int  GameTree::minimax(GTreeNode* node, int depth, bool maximizingPlayer)
{
    std::vector<GTreeNode*>::iterator it;

    if( depth==0 || node->isLeaf() )
        return node->value;

    if(bSlowMode)
    {
       // usleep(500000);
       // QApplication::processEvents();
       // emit redrawSig();
    }

    if(maximizingPlayer == true)
    {
        int bestVal = -INF;
        for(it=node->children.begin(); it!=node->children.end(); it++)
        {
            int value = minimax((*it), depth-1, false);
            bestVal = std::max(bestVal,value);
        }
        if( !node->isLeaf() )
            node->value = bestVal;
        return bestVal;
    }
    else if(maximizingPlayer == false)
    {
        int bestVal = INF;
        for(it=node->children.begin(); it!=node->children.end(); it++)
        {
            int value = minimax((*it), depth-1, true);
            bestVal = std::min(bestVal,value);
        }
        if( !node->isLeaf() )
           node->value = bestVal;
        return bestVal;
    }


}

int GameTree::alphabeta(GTreeNode* node, int depth, int a, int b, bool maximizingPlayer)
{
    std::vector<GTreeNode*>::iterator it;
    if( depth==0 || node->isLeaf() )
        return node->value;

    if(bSlowMode)
    {
        //usleep(500000);
        //QApplication::processEvents();
        //emit redrawSig();
    }

    if(maximizingPlayer == true)
    {
        for(it=node->children.begin(); it!=node->children.end(); it++)
        {
            a = std::max(a,alphabeta(*it,depth-1,a,b,false));
            node->alpha = a;
            node->beta = b;
            if( a >= b )
            {
                break;
            }
        }
        if( !node->isLeaf() )
          node->value = a;
        return a;
    }
    else if(maximizingPlayer == false)
    {
        for(it=node->children.begin(); it!=node->children.end(); it++)
        {
            b = std::min(b,alphabeta(*it,depth-1,a,b,true));
            node->alpha = a;
            node->beta = b;
            if( a >= b )
            {
                break;
            }
        }
        if( !node->isLeaf() )
          node->value = b;
        return b;
    }

}



void GameTree::MiniMaxOnly(GTreeNode* node)
{
    std::vector<GTreeNode*>::iterator it;

    if(!node->isLeaf())
    {
        if( node->type == MAXER)
            node->value = chooseMax(node->children);
        if( node->type == MINER)
            node->value = chooseMin(node->children);
    }

    for(it=node->children.begin(); it!=node->children.end(); it++)
       MiniMaxOnly(*it);

    if(bSlowMode)
    {
        usleep(50000);
        QApplication::processEvents();
        emit redrawSig();
    }
}

void GameTree::SetLevels(GTreeNode* node)
{
    std::vector<GTreeNode*>::iterator it;

    for(it=node->children.begin(); it!=node->children.end(); it++)
    {
       (*it)->level = node->level + 1;
       SetLevels(*it);
    }

}

GTreeNode* GameTree::getNodeByID(int id, GTreeNode* node, GTreeNode* found_node)
{
    if(id == node->id)
    {
      found_node = node;
      return found_node;
    }

    if(node->children.size()==0 )
        return found_node;

    for(auto i=node->children.begin(); i!=node->children.end(); i++)
      found_node = getNodeByID(id, (*i), found_node);

    return found_node;


    //if(id == node->id)
    //    return node;
}

void GameTree::free_mem(GTreeNode* root)
{
    for(auto i=root->children.begin(); i!=root->children.end(); i++)
    {
       free_mem((*i));
       if( (*i)->isLeaf() )
           delete (*i);

    }
}
