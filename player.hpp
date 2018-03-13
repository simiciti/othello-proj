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
    Move *iterMax(Board *board, double (Player::*eval)(Board *));
    Move *doSimpleMove(Move *opponentsMove, int msLeft);
    Move *doCornerMove(Move *opponentsMove, int msLeft);
    Move *doGreedyMove(Move *opponentsMove, int msLeft);
    Move *doMobilityMove(Move *opponentsMove, int msLeft);
    
    
    Move *doLimitMove(Move *opponentsMove, int msLeft);

    void setBoard(char data[]);
    
    
    
    double depth2_minimax(Board *board, int depth, bool isMax, Side side,
                          int msLeft, int limit);
    Move* minimax_move(Board *board, int depth, bool isMax, Side side, 
                          int msLeft, int limit);
    
    
    double depth2_eval(Board *board);
    
    Move *alphabetaInitial(int depth, bool isMax, int msLeft);
    double alphabeta(Board *board, int depth, double alpha, double beta, 
                 bool isMax, Side side, Move *move);
    int mobility_eval(Board *board, Side side);
    double evaluate(Board *board);
    double evaluateCornerCloseness(Board *board);
    double evaluateMobility(Board *board);
    double evaluateCoins(Board *board);
    double evaluateWeightedCoins(Board *board);
    double getWeight(int x, int y);
    double cat_eval(double max_val, double min_val);
    
    
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;

private:
	Side mySide;
	Side oppSide;
	Board *board;
};

#endif
