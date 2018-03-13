#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include <unordered_map>
#include "common.hpp"
using namespace std;

class Board {

private:
    bitset<64> black;
    bitset<64> taken;
    
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);
    
    


public:
    Board();
    ~Board();
    Board *copy();
    bool occupied(int x, int y);
    void copyFromBoard(Board *otherBoard);
    bool get(Side side, int x, int y);
    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    int count(Side side);
    int countMoves(Side side);
    int countBlack();
    int countWhite();

    bitset<128> hsh();
    //transposition table
    unordered_map<bitset<128>, double> table;
    void setBoard(char data[]);
    void printBoard();
};

#endif
