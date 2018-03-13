#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <unordered_map>
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
    
    
    
    double minimax(Board *board, int depth, bool isMax, Side side, double *alpha, double *beta,
                          int msLeft, int limit, bool isTest, double *eval);
    Move* minimax_move(Board *board, int depth, bool isMax, Side side, 
                          int msLeft, int limit, bool isTest);
    
    
    double depth2_eval(Board *board);
    
    

    int mobility_eval(Board *board, Side side);
    double evaluate(Board *board, double *eval);
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
    //transposition table
    unordered_map<bitset<64>, double> table;
    
};

#endif
