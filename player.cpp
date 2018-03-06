#include "player.hpp"
#include <cstdlib>
#include <stdio.h>
#include <cmath>

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
 * @brief loads the representation into data
 * 
 * @param data[] - char array of data representing a board
 */
void Player::setBoard(char data[])
{
    this->board->setBoard(data);
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
    if (this->testingMinimax)
    {
        return minimax_move(this->board, 0, true, this->mySide, msLeft, 0);
    }
    else
    {
        
        return doLimitMove(opponentsMove, msLeft);
    }
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

    float best_score = -100;
    Move cornerMost = Move(-1, -1);
    Move besideEdge = Move(-1, -1);
    Move curr = Move(0, 0);
    float curr_score;
    // Iterate through all possible moves and determine legal move
    // closest to a corner. 
    for (int x = 0; x < 8; x++) {
        curr.setX(x);
        for (int y = 0; y < 8; y++) {
            curr.setY(y);
            if (this->board->checkMove(&curr, this->mySide)) {
                if (x == 2 || x == 7 || y == 2 || y == 7) {
                    besideEdge.setX(x);
                    besideEdge.setY(y);
                }
                curr_score = max(abs(x - 3.5), abs(y - 3.5));
                if (best_score < curr_score ) {
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
        if(besideEdge.getX() == -1)
            return nullptr;
        else {
            this->board->doMove(&besideEdge, this->mySide);
            return new Move(besideEdge.getX(), besideEdge.getY());
        }
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
                curr_score = testBoard.count(this->mySide) - 
                testBoard.count(this->oppSide);
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

/**
 * @brief Perform move minimizing opponent's possible moves.
 */
Move *Player::doLimitMove(Move *opponentsMove, int msLeft) {
    if (opponentsMove != nullptr)
        this->board->doMove(opponentsMove, oppSide);

    int best_score = -65;
    Move mobileMove = Move(-1, -1);
    Move curr = Move(0, 0);
    int curr_score;
    Board testBoard = Board();

    // Iterate through all possible moves and determine legal move
    // which yields the leastest possible moves for opponent.
    for (int x = 0; x < 8; x++) {
        curr.setX(x);
        for (int y = 0; y < 8; y++) {
            curr.setY(y);
            if (this->board->checkMove(&curr, this->mySide)) {
                testBoard.copyFromBoard(this->board);
                testBoard.doMove(&curr, this->mySide);
                curr_score = - testBoard.countMoves(this->oppSide);
                testBoard.count(this->oppSide);
                if (curr_score > best_score) {
                    mobileMove.setX(x);
                    mobileMove.setY(y);
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
        this->board->doMove(&mobileMove, this->mySide);
        return new Move(mobileMove.getX(), mobileMove.getY());
    }
}

/**
 * @brief Board evaluator for minimax function using a simple coin 
 * counting heuristic 
 * 
 * @return the difference in coins 
 */
double Player::depth2_eval(Board *board)
{
    return board->count(this->mySide) - board->count(this->oppSide);
}


/**
 * @brief The minimax algorithm. This will recursivey call
 * the algorithm for the player and the opponent to a depth of limit 
 * 
 * @param board - a copy of the current board state in the tree
 * @param depth - the current depth in the tree. Should advance after a 
 * player - opponent cycle.
 * @param isMax - are we seeking to maximize utiity (player) or minimize it 
 * (opponent)?
 * @param side - the current side for the entity calculating 
 * @param msLeft- miliseconds left 
 * @param limit - the depth to search before returnining 
 * 
 * @return the calculated utility for that branch
 */
double Player::depth2_minimax(Board *board, int depth, bool isMax,
                              Side side, int msLeft, int limit)
{
    if (depth > limit)
    {
        return depth2_eval(board);
    }
    else
    {
        Board br_board = Board();
        br_board.copyFromBoard(board);
        
        double minimax = (isMax) ? -100 : 100;
        Move *m = new Move(0, 0);
        
        for (int i = 0; i < 8; i++) {
            m->setX(i);
            for (int j = 0; j < 8; j++) {
                m->setY(j);
                if (board->checkMove(m, side)) {
                    
                    br_board.doMove(m, side);
                    double path_value;
                    Side other = (side == BLACK) ? WHITE : BLACK;
                    if (this->mySide == side)
                    {
                        path_value = depth2_minimax(&br_board, depth, 
                                                !isMax, other, msLeft, limit);
                    }
                    else //this is a hypothetical calculation for the opponent 
                    {
                        path_value = depth2_minimax(&br_board, depth + 1, !isMax,
                                                other, msLeft, limit);
                    }
                    if (isMax ^ (path_value < minimax))
                    {
                        minimax = path_value;
                    }
                    br_board.copyFromBoard(board);
                }
            }
        }
        return minimax;
    }
}

/**
 * @brief Level 0 for the minimax algorithm. This will recursivey call
 * the algorithm for the player and the opponent to a depth of limit 
 * 
 * @param board - a copy of the current board state in the tree
 * @param depth - the current depth in the tree. Should advance after a 
 * player - opponent cycle.
 * @param isMax - are we seeking to maximize utiity (player) or minimize it 
 * (opponent)?
 * @param side - the current side for the entity calculating 
 * @param msLeft- miliseconds left 
 * @param limit - the depth to search before returnining 
 * 
 * @return the calculated best move
 */
Move *Player::minimax_move(Board *board, int depth, bool isMax, 
                              Side side, int msLeft, int limit)
{
        if (!this->board->hasMoves(this->mySide))
        {
            return nullptr;
        }
        
        double minimax = (isMax) ? -100 : 100;
        
        int xcor;
        int ycor;
        
        Move *m = new Move(0, 0);
    
        Board br_board = Board();
        br_board.copyFromBoard(board);
        for (int i = 0; i < 8; i++) {
            m->setX(i);
            for (int j = 0; j < 8; j++) {
                m->setY(j);
                if (board->checkMove(m, side)) {
                    br_board.doMove(m, side);
                    
                    double path_value;
                    Side other = (side == BLACK) ? WHITE : BLACK;
                    if (this->mySide == side)
                    {
                        path_value = depth2_minimax(&br_board, depth,
                                                !isMax, other, msLeft, limit);
                    }
                    else //this is a hypothetical calculation for the opponent 
                    {
                        path_value = depth2_minimax(&br_board, depth + 1, 
                                                !isMax, other, msLeft, limit);
                    }
                    if (isMax ^ (path_value < minimax))
                    {
                        minimax = path_value;
                        xcor = i;
                        ycor = j;
                    }
                    br_board.copyFromBoard(board); 
                }
            }
        }
       
        if (depth == 0 && isMax)
        {
            m->setX(xcor);
            m->setY(ycor);
        }
        return m;
}
/**
 * @brief Skeleton for a more complex evaluation function
 */
double Player::evaluate(Board *board)
{
    //corners not final name 
    //double corners_component  = cat_eval(corners(1), corners(0));
    //double stability_component = cat_eval(stability(1), stability(0));
    //double coins_component = cat_eval(coins(1), coins(0));
    //double mobility_component = cat_eval(mobility(1), mobility(0));
    
    double corners_weight = 0.3;
    double mobility_weight = 0.05;
    double stability_weight = 0.25;
    double coins_weight = 0.25;
    return corners_weight + mobility_weight + stability_weight + coins_weight;
}
/**
 * category agnostic evaluation function
 * @param max_val - the value for the player who will move, 
 * desires maximization of utility
 * @param min_val - the value for the player who desires the 
 * minimization of utility for the max player
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

