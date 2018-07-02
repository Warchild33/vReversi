#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <map>

enum CellState {NIL=0, FREE=1, WHITE=2, BLACK=3, MARKED=4} ;
enum MoveType {COMPUTER=0, PLAYER=1} ;

struct Cell
{
    int i;
    int j;
    CellState state;
    Cell()
    {
        i=-1; j=-1; state = NIL;
    }

    Cell(int i, int j, CellState state)
        :i(i),j(j),state(state)
    {}
    inline bool black_or_white()
    {
        return (state == WHITE) || (state == BLACK);
    }
    inline CellState InversedState()
    {
        if( state == WHITE  )
            return BLACK;
        if( state == BLACK  )
            return WHITE;
        return state;
    }
};

class Board : public QObject
{
    Q_OBJECT
public:
    explicit Board(QObject *parent = 0);
    bool hasNeibours(Cell* cell);
    Cell* getCell(int i, int j);
    int   getN() { return N; }
    void  setInitalState();
    bool  fillLine(Cell* cell);
    void  fillLine(std::vector<Cell*>& line);
    bool  checkLine(Cell* cell);
    void  markCells(std::vector<Cell*>&list);
    std::vector<Cell*> walkLine(Cell* cell, int dx, int dy, bool& bFilled);
    void CountScore(int& n_black, int& n_white);
    std::vector<Cell*> selectCellsWithNeibours();
    void ResetMarked();
    int  getValue();
    void save();
    void saveToArray(std::vector<int>& states);
    void loadFromArray(std::vector<int> states);
    void restore();


    int N;

    Cell nil_cell;
    std::vector<Cell*> cells;
    std::vector<Cell*> cells_states0;
    std::map<std::pair<int,int>, Cell* > cell_map;

signals:

public slots:
};





#endif // BOARD_H
