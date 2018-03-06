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

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;

private:
	Side mySide;
	Side oppSide;
	Board *board;
};

#endif
