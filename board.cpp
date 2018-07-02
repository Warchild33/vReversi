#include "board.h"

Board::Board(QObject *parent) : QObject(parent)
{
    N = 8;
    for(int j=0; j<N; j++)
    for(int i=0; i<N; i++)
    {
        Cell* cell = new Cell(i,j,FREE);
        Cell* cell2 = new Cell(i,j,FREE);
        cells.push_back(cell);
        cells_states0.push_back(cell2);
        cell_map[std::make_pair(i,j)] = cell;
    }
    setInitalState();
}

void Board::save()
{
    for(auto i=cells.begin(),j=cells_states0.begin(); i!=cells.end(); i++,j++)
        (*j)->state = (*i)->state;
}

void Board::restore()
{
    for(auto i=cells.begin(),j=cells_states0.begin(); i!=cells.end(); i++,j++)
        (*i)->state = (*j)->state;
}

void Board::ResetMarked()
{
    for(auto i=cells.begin(); i!=cells.end(); i++)
        if( (*i)->state == CellState::MARKED )
            (*i)->state = CellState::FREE;
}

void Board::CountScore(int& n_black, int& n_white)
{
    n_black = 0;
    n_white = 0;
    for(int i=0; i < getN(); i++)
    for(int j=0; j < getN(); j++)
    {
        Cell* cell = getCell(i, j);
        if(cell->state == CellState::BLACK)
            n_black++;
        if(cell->state == CellState::WHITE)
            n_white++;
    }
}


Cell* Board::getCell(int i, int j)
{
    std::pair<int,int> key = std::make_pair(i,j);
    if( cell_map.find(key)!=cell_map.end() )
    {
        return cell_map[key];
    }
    else
        return &nil_cell;
}

void  Board::setInitalState()
{
    getCell(3,3)->state = CellState::WHITE;
    getCell(4,3)->state = CellState::BLACK;
    getCell(3,4)->state = CellState::BLACK;
    getCell(4,4)->state = CellState::WHITE;
}

bool Board::hasNeibours(Cell* cell)
{
    if( getCell(cell->i+1, cell->j)->black_or_white()  ) return true;
    if( getCell(cell->i, cell->j+1)->black_or_white()  ) return true;
    if( getCell(cell->i+1, cell->j+1)->black_or_white()  ) return true;
    if( getCell(cell->i-1, cell->j-1)->black_or_white()  ) return true;
    if( getCell(cell->i, cell->j-1)->black_or_white()  ) return true;
    if( getCell(cell->i-1, cell->j)->black_or_white()  ) return true;
    if( getCell(cell->i-1, cell->j+1)->black_or_white()  ) return true;
    if( getCell(cell->i+1, cell->j-1)->black_or_white()  ) return true;
    return false;
}

std::vector<Cell*> Board::selectCellsWithNeibours()
{
    std::vector<Cell*> list;
    for(int i=0; i < getN(); i++)
    for(int j=0; j < getN(); j++)
    {
        Cell* cell = getCell(i, j);
        if( (cell->state == CellState::FREE) && hasNeibours(cell) )
            list.push_back(cell);
    }
    return list;
}

void Board::saveToArray(std::vector<int>& states)
{
    for(auto i=cells.begin(); i!=cells.end(); i++)
        states.push_back( (*i)->state );
}

void Board::loadFromArray(std::vector<int> states)
{
    auto j=states.begin();
    for(auto i=cells.begin(); i!=cells.end(); i++,j++)
        (*i)->state = (CellState)(*j);
}


int  Board::getValue()
{
    int n_black, n_white;
    CountScore(n_black, n_white);
    return n_black-n_white;
}

void  Board::markCells(std::vector<Cell*>& list)
{
    for(auto i=list.begin(); i!=list.end(); i++)
    {
        (*i)->state = CellState::MARKED;
    }
}


std::vector<Cell*> Board::walkLine(Cell* cell, int dx, int dy,bool& bFilled)
{
    std::vector<Cell*> line;
    Cell* cur_cell = getCell(cell->i+dx, cell->j+dy);
    line.push_back(cell);
    CellState inversed_state = cell->InversedState();
    while( cur_cell->state == inversed_state )
    {
        line.push_back(cur_cell);
        cur_cell = getCell(cur_cell->i+dx, cur_cell->j+dy);
        //if(!cur_cell->black_or_white()) break;
        if(cur_cell->state == CellState::FREE) break;
        if(cur_cell->state == CellState::NIL) break;
    }
    if(cur_cell->state!=cell->state)
        line.clear();
    if(line.size()>1) bFilled = true; //else bFilled = false;
    return line;
}

void Board::fillLine(std::vector<Cell*>& line)
{
    if( line.size() < 2 ) return;
    CellState state = line[0]->state;
    for(auto i=line.begin(); i!=line.end(); i++)
    {
        (*i)->state = state;
    }
}

bool Board::fillLine(Cell* cell)
{
   bool bFilled=false;
   //up
   std::vector<Cell*> line = walkLine(cell,0,-1,bFilled);
   fillLine(line);
   //down
   line = walkLine(cell,0,1,bFilled);
   fillLine(line);
   //left
   line = walkLine(cell,-1,0,bFilled);
   fillLine(line);
   //right
   line = walkLine(cell,1,0,bFilled);
   fillLine(line);
    //TL
   line = walkLine(cell,1,-1,bFilled);
   fillLine(line);
   //TR
   line = walkLine(cell,-1,-1,bFilled);
   fillLine(line);
   //BR
   line = walkLine(cell,-1,1,bFilled);
   fillLine(line);
   //BL
   line = walkLine(cell,1,1,bFilled);
   fillLine(line);
   return bFilled;
}

bool Board::checkLine(Cell* cell)
{
   bool bFilled=false;
   //up
   walkLine(cell,0,-1,bFilled);
   //down
   walkLine(cell,0,1,bFilled);
   //left
   walkLine(cell,-1,0,bFilled);
   //right
   walkLine(cell,1,0,bFilled);
    //TL
   walkLine(cell,1,-1,bFilled);
   //TR
   walkLine(cell,-1,-1,bFilled);
   //BR
   walkLine(cell,-1,1,bFilled);
   //BL
   walkLine(cell,1,1,bFilled);
   return bFilled;
}
