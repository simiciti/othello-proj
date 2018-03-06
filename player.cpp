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

/**
 * @brief Board evaluator for minimax function using a simple coin counting heuristic 
 * 
 * @return the difference in coins 
 */
double Player::depth2_eval(Board *board)
{
    return board->count(this->mySide) - board->count(this->oppSide);
}


double Player::depth2_minimax(Board *board, int depth, boolean isMax, Side side)
{
    if (depth == 2 && this-> mySide != side)
    {
        return depth2_eval(board);
    }
    else
    {
        Board *br_board = board->copy();
        double minimax = 0;
        int xcor;
        int ycor;
        Move *m = new Move(0, 0);

        for (int i = 0; i < 8; i++) {
            m->setX(i);
            for (int j = 0; j < 8; j++) {
                m->setY(j);
                if (br_board->checkMove(m, side)) {
                    br_board->doMove(m, side);
                    double path_value;
                    Side other = (side == BLACK) ? WHITE : BLACK;
                    if (this->mySide == side)
                    {
                        path_value = depth2_minimax(br_board, depth + 1, !isMax, other);
                    }
                    else //this is a hypothetical calculation for the opponent 
                    {
                        path_value = depth2_minimax(br_board, depth, !isMax, other);
                    }
                    if (isMax ^ path_value < minimax)
                    {
                        minimax = path_value;
                        xcor = i;
                        ycor = j;
                    }
                }
            }
        }
        delete br_board;
        return minimax;
    }
}

double Player::depth2_minimax(Board *board, int depth, boolean isMax, Side side, int *x, int *y)
{
    if (depth == 2 && this-> mySide != side)
    {
        return depth2_eval(board);
    }
    else
    {
        Board *br_board = board->copy();
        double minimax = 0;
        int xcor;
        int ycor;
        Move *m = new Move(0, 0);

        for (int i = 0; i < 8; i++) {
            m->setX(i);
            for (int j = 0; j < 8; j++) {
                m->setY(j);
                if (br_board->checkMove(m, side)) {
                    br_board->doMove(m, side);
                    double path_value;
                    Side other = (side == BLACK) ? WHITE : BLACK;
                    if (this->mySide == side)
                    {
                        path_value = depth2_minimax(br_board, depth + 1, !isMax, other);
                    }
                    else //this is a hypothetical calculation for the opponent 
                    {
                        path_value = depth2_minimax(br_board, depth, !isMax, other);
                    }
                    if (isMax ^ path_value < minimax)
                    {
                        minimax = path_value;
                        xcor = i;
                        ycor = j;
                    }
                }
            }
        }
        delete m;
        delete br_board;
        if (depth == 0)
        {
            *x = xcor;
            *y = ycor;
        }
        return minimax;
    }
}
/**
 * 
 */
double Player::evaluate(Board *board)
{
    //corners not final name 
    double corners_component  = cat_eval(corners(1), corners(0));
    double stability_component = cat_eval(stability(1), stability(0));
    double coins_component = cat_eval(coins(1), coins(0));
    double mobility_component = cat_eval(mobility(1), mobility(0));
    
    double corners_weight = 0.3;
    double mobility_weight = 0.05;
    double stability_weight = 0.25;
    double coins_weight = 0.25;
}
/**
 * category agnostic evaluation function
 * @param max_val - the value for the player who will move, desires maximization of utility
 * @param min_val - the value for the player who desires the minimization of utility for the max player
 */
double Player::cat_eval(double max_val, double min_val)
{
    if (max_val + min_val != 0 )
    {
        return (max_val - min_val) / (max_val + min_val);
    }
    else
    {
       return 0.0; 
    }
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
    if (this->board->hasMoves(this->mySide))
    {
        if (this->testingMinimax)
        {
            int x;
            int y;
            depth2_minimax(this->board, 0, true, this->mySide, &x, &y);
            m->setX(x);
            m->setY(y);
            return m;
        }
    }
    /**
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
    */

    return nullptr;
}
