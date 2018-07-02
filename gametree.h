#ifndef GAMETREE_H
#define GAMETREE_H

#include <vector>
#include <QRectF>
#include <QObject>

#define INF 100000000
#define MAXER 0
#define MINER 1

class GTreeNode
{
public:
    GTreeNode(bool type)
    {
       if( type == MAXER )
        value = -INF;
       else
        value = INF;
       this->type = type;
       level=0;
       alpha = -INF;
       beta = INF;
       id = GenerateID();
    }
    GTreeNode(bool type,int value)
    {
       this->value = value;
       this->type = type;
       level=0;
       alpha = -INF;
       beta = INF;
       id = GenerateID();
    }
    bool operator>(GTreeNode* B)
    {
        return value >  B->value;
    }
    bool operator<(GTreeNode* B)
    {
        return value <  B->value;
    }
    bool operator==(GTreeNode* B)
    {
        return value ==  B->value;
    }
    void setLevel(int l){ level=l;}
    int GenerateID()
    {
        return rand();
    }

    QRectF rect;
    int id;
    int value;
    int alpha;
    int beta;
    int level;
    bool type;
    std::vector<int> game_state;
    std::vector<GTreeNode*> children;
    void AddChild(GTreeNode* child){ children.push_back(child); }
    bool isLeaf(){ return children.size() == 0; }
    void setRect(QRectF rc){ rect = rc; }

};

class GameTree : public QObject
{
    Q_OBJECT
public:
    GTreeNode* root;
    GameTree();
    bool bSlowMode;

    void CreateTestTree();
    GTreeNode* getNodeByID(int id, GTreeNode* node, GTreeNode* fnode);
    void MiniMaxOnly(GTreeNode* node);
    void MiniMax(GTreeNode* node);
    int  alphabeta(GTreeNode* node, int depth, int a, int b, bool maximizingPlayer);
    int  minimax(GTreeNode* node, int depth, bool maximizingPlayer);
    void SetLevels(GTreeNode* node);
    void SetSlowMode(){ bSlowMode = true; }
    void StartMiniMax();
    GTreeNode* mouseHit(GTreeNode* root, int x, int y);
    void Reset();
    void MakeReset(GTreeNode* node);
    void free_mem(GTreeNode* root);

signals:
    void redrawSig();

};

#endif // GAMETREE_H
