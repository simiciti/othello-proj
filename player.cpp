#include "player.hpp"
#include <cstdlib>
#include <stdio.h>

/**
 * This specific file is in the repository of Nathaniel Smith
 * This is an (initially simple) player for a computerized version of the game othello.
 * 
 */


/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    //how much time available for computation is there?
    int time_remaining = 0;

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
    // Set our side and opponent's side
    this->mySide = side;
    (side == BLACK) ? this->oppSide = WHITE : this->oppSide = BLACK;
    this->board = new Board();
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete this->board;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */

    if (opponentsMove != nullptr) {
        this->board->doMove(opponentsMove, oppSide);
    }

    Move *m = new Move(0, 0);

    for (int i = 0; i < 8; i++) {
        m->setX(i);
        for (int j = 0; j < 8; j++) {
            m->setY(j);
            if (this->board->checkMove(m, this->mySide)) {
                this->board->doMove(m, this->mySide);
                return m;
            }
        }
    }

    return nullptr;
}
