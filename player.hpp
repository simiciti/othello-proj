#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();

    double depth2_eval(Board *board);
    //int depth, boolean isMax, Board *board, Side side
    double depth2_minimax(Board *board, int depth, bool isMax, Side side);
    double depth2_minimax(Board *board, int depth, bool isMax, Side side, int *x, int *y);
    
    
    double evaluate(Board *board);
    double cat_eval(double max_val, double min_val);
    
    Move *doMove(Move *opponentsMove, int msLeft);
    
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;

private:
	Side mySide;
	Side oppSide;
	Board *board;
};

#endif
