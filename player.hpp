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

    Move *doMove(Move *opponentsMove, int msLeft);
    Move *doSimpleMove(Move *opponentsMove, int msLeft);
    Move *doCornerMove(Move *opponentsMove, int msLeft);
    Move *doGreedyMove(Move *opponentsMove, int msLeft);

    void setBoard(char data[]);
    
    double depth2_eval(Board *board);
    
    double depth2_minimax(Board *board, int depth, bool isMax, Side side,
                          int msLeft, int limit);
    Move* minimax_move(Board *board, int depth, bool isMax, Side side, 
                          int msLeft, int limit);
    
    
    double evaluate(Board *board);
    double cat_eval(double max_val, double min_val);
    
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;

private:
	Side mySide;
	Side oppSide;
	Board *board;
};

#endif
