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

    return doSimpleMove(opponentsMove, msLeft);
}

/**
 * @brief Iterate through moves and perform first legal move.
 */
Move *Player::doSimpleMove(Move *opponentsMove, int msLeft) { 
    // Perform opponent's move
    if (opponentsMove != nullptr)
        this->board->doMove(opponentsMove, oppSide);

    Move *m = new Move(0, 0);
    // Iterate through all possible moves and return first legal move.
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

    // If no move was found, return nullptr. 
    return nullptr;
}

/**
 * @brief Perform move closest to a corner.
 */
Move *Player::doCornerMove(Move *opponentsMove, int msLeft) {
    // Perform opponent's move
    if (opponentsMove != nullptr)
        this->board->doMove(opponentsMove, oppSide);

    float best_score = 0;
    Move cornerMost = Move(-1, -1);
    Move curr = Move(0, 0);
    float curr_score;
    // Iterate through all possible moves and determine legal move
    // closest to a corner. 
    for (int x = 0; x < 8; x++) {
        curr.setX(x);
        for (int y = 0; y < 8; y++) {
            curr.setY(y);
            if (this->board->checkMove(&curr, this->mySide)) {
                curr_score = (x - 3.5) * (x - 3.5) + (y - 3.5) * (y - 3.5);
                if (best_score < curr_score) {
                    cornerMost.setX(x);
                    cornerMost.setY(y);
                    best_score = curr_score;
                }
            }
        }
    }

    // If no move was found, return nullptr. 
    // Otherwise, return the move.
    if (cornerMost.getX() == -1)
        return nullptr;
    else {
        this->board->doMove(&cornerMost, this->mySide);
        return new Move(cornerMost.getX(), cornerMost.getY());
    }
}

/**
 * @brief Perform move yielding the greatest difference in score. 
 */
Move *Player::doGreedyMove(Move *opponentsMove, int msLeft) {
    if (opponentsMove != nullptr)
        this->board->doMove(opponentsMove, oppSide);

    int best_score = -65;
    Move greedyMove = Move(-1, -1);
    Move curr = Move(0, 0);
    int curr_score;
    Board testBoard = Board();

    // Iterate through all possible moves and determine legal move
    // which yields the greatest difference in score.  
    for (int x = 0; x < 8; x++) {
        curr.setX(x);
        for (int y = 0; y < 8; y++) {
            curr.setY(y);
            if (this->board->checkMove(&curr, this->mySide)) {
                testBoard.copyFromBoard(this->board);
                testBoard.doMove(&curr, this->mySide);
                curr_score = testBoard.count(this->mySide) - testBoard.count(this->oppSide);
                if (curr_score > best_score) {
                    greedyMove.setX(x);
                    greedyMove.setY(y);
                    best_score = curr_score;
                }
            }
        }
    }

     // If no move was found, return nullptr. 
    // Otherwise, return the move.
    if (best_score == -65)
        return nullptr;
    else {
        this->board->doMove(&greedyMove, this->mySide);
        return new Move(greedyMove.getX(), greedyMove.getY());
    }
}
