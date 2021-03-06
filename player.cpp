#include "player.hpp"
#include <cstdlib>
#include <stdio.h>
#include <cmath>
#include <chrono>
#include <unordered_map>

#define symMove00 300.125
#define symMove10 189.125
#define symMove20 155.125
#define symMove30 150.125
#define symMove11 78.125
#define symMove21 44.125
#define symMove31 39.125
#define symMove22 10.125
#define symMove32 5.125
#define symMove33 0.125

/**
 * This is an (initially simple) player for a computerized 
 * version of the game othello.
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

/**
 * @brief Provides an approximate time allocation for a search 
 * 
 * @param msLeft - the time left 
 * @param move - the current move (number of pieces on board - 4)
 * 
 * @return a time allocation
 */
int Player::timeAllocation(int msLeft, int move)
{
    // int alloc = (msLeft * (move - 4)) / 60;
    int alloc = msLeft;
    //the first run
    if (alloc <= 0)
    {
        return msLeft / 64;
    }
    else
    {
        return alloc;
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
    if (opponentsMove != nullptr)
          {
              this->board->doMove(opponentsMove, oppSide);
          }
    
    if (this->testingMinimax)
    {
        return minimaxMove(this->board, 0, true, this->mySide, 
                                   msLeft, 1, true);
    }
    else 
    {
        Move *m =  minimaxMove(this->board, 0, true,
                               this->mySide, msLeft, 1, false);
        this->board->doMove(m, this->mySide);
        return m;
    }
}

/**
 * @brief returns the best possible move on given board with given evaluate 
 *        functions (returns nullptr if no move is possible). 
 */
Move *Player::bestMoveWith(Board *board, double (Player::*eval)(Board *)) {
    Move bestMove = Move(-1, -1);
    Move currMove = Move(0, 0);
    double bestScore = -1000;
    double currScore;
    Board testBoard = Board();

    // Iterate through all possible moves and determine legal move
    // which yields the highest evaluation.
    for (int x = 0; x < 8; x++) {
        currMove.setX(x);
        for (int y = 0; y < 8; y++) {
            currMove.setY(y);
            // If move is available, evaluate for that move
            // and update the best move and score accordingly.
            if (board->checkMove(&currMove, this->mySide)) {
                testBoard.copyFromBoard(board);
                testBoard.doMove(&currMove, this->mySide);
                currScore = (this->*eval)(&testBoard);
                if (currScore > bestScore) {
                    bestMove.setX(x);
                    bestMove.setY(y);
                    bestScore = currScore;
                }
            }
        }
    }

    if (bestMove.getX() == -1)
        return nullptr;
    else
        return new Move(bestMove.getX(), bestMove.getY());
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


Move *Player::doMobilityMove(Move *opponentsMove, int msLeft)
{
     if (opponentsMove != nullptr)
     {
        this->board->doMove(opponentsMove, oppSide);
     }
    Board moves_board = Board();
    moves_board.copyFromBoard(this->board);
    
    Move *m = new Move(1,1);
    double state = -65;
    int xcor = 0;
    int ycor = 0;
    for (int i = 0; i < 8; i++) {
            m->setX(i);
            for (int j = 0; j < 8; j++) {
                m->setY(j);
                if (board->checkMove(m, this->mySide)) {
                    moves_board.doMove(m, this->mySide);
                    
                    int opp_moves = mobility_eval(&moves_board, this->oppSide);
                    int moves = mobility_eval(&moves_board, this->mySide);
                    double eval = cat_eval(moves, opp_moves);
                    if (eval > state)
                    {
                        state = eval;
                        xcor = i;
                        ycor = j;
                    }
                    
                }
            }
        }
    if (state != -65)
    {
        m->setX(xcor);
        m->setY(ycor);
        return m;
    }
    else
    {
        return nullptr;
    }
     
     
}

/**
 * @brief Calculates and returns the number of moves for a given side
 */
int Player::mobility_eval(Board *board, Side side)
{
    Move *m = new Move(1,1);
    int moves = 0;
    for (int i = 0; i < 8; i++) {
            m->setX(i);
            for (int j = 0; j < 8; j++) {
                m->setY(j);
                if (board->checkMove(m, side)) {
                    moves++;
                }
            }
        }
        return moves;
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
    // return board->count(this->mySide) - board->count(this->oppSide);
    return evaluateWeightedCoins(board);
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
 * @param alpha - the alpha value
 * @param beta - the beta value
 * @param msLeft- miliseconds left 
 * @param limit - the depth to search before returnining 
 * 
 * @return the calculated utility for that branch
 */
double Player::minimax(Board *board, int depth, bool isMax,
    Side side, double *alpha, double *beta, int msLeft, int limit, bool isTest)
{
    if (depth > limit)
    {
        if (isTest)
        {
            return depth2_eval(board);
        }
        //cerr << "depth: " << depth << endl;
        return evaluate(board);
    }
    else
    {
        Board br_board = Board();
        br_board.copyFromBoard(board);
        
        double value = (isMax) ? -100 : 100;
        Move *m = new Move(0, 0);
        double path_value;
        Side other = (side == BLACK) ? WHITE : BLACK;
        
        for (int i = 0; i < 8; i++) {
            m->setX(i);
            for (int j = 0; j < 8; j++) {
                m->setY(j);
                if (board->checkMove(m, side)) {
                    
                    br_board.doMove(m, side);
                    bitset<128> hash = br_board.hsh();
                    if (this->mySide == side)
                    {
                        
                        if (false)//this->board->table[hash])
                        {
                            path_value = this->board->table[hash];
                        }
                        else
                        {
                        path_value = minimax(&br_board, depth, 
                        !isMax, other, alpha, beta, msLeft, limit, isTest);
                         this->board->table[hash] = path_value;
                         std::cerr << path_value << std::endl;
                        }
                        value = std::max(value, path_value);
                        *alpha = std::max(*alpha, value);
                    }
                    else //this is a hypothetical calculation for the opponent 
                    {
                        if (false) //this->board->table[hash])
                        {
                            path_value = this->board->table[hash];
                        }
                        else
                        {
                        path_value = minimax(&br_board, depth + 1, !isMax,
                                other, alpha, beta, msLeft, limit, isTest);
                        this->board->table[hash] = path_value;
                        }
                        value = std::min(value, path_value);
                        *beta = std::min(*beta, value);
                    }
                    if (beta <= alpha)
                    {
                        break;
                    }
                    br_board.copyFromBoard(board);
                }
            }
        }
        return value;
    }
}

/**
 * @brief Level 0 for the minimax algorithm. This will recursively call
 * the algorithm for the player and the opponent to a depth of limit 
 * 
 * Absolutely gimped iterative deepening 
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
Move *Player::minimaxMove(Board *board, int depth, bool isMax, 
                              Side side, int msLeft, int limit, bool isTest)
{
        double value = (isMax) ? -100 : 100;
        // auto start = std::chrono::system_clock::now();
        int xcor = -1;
        int ycor;
        /*
        Testing suggestst this happens if nothing was passed to
        Player::doMove() for msLeft, such as the case of an untimed
        game. Am aware that in actual tournament play, may be case 
        of msLeft truly being 0, but odds of exactly 0 deemed low enough
        that this case needn't be considered.
        */

        int allocation = timeAllocation(msLeft, board->count(this->mySide) 
        + board->count(this->oppSide));
        if (msLeft <= 0)
        {
            allocation = 1000000;
        }
        int used = 0;

        Move *m = new Move(0, 0);
        Move *l = new Move(0, 0);
        for (int n = limit - 1 ; n < limit + 1; n++)
        {
        //     if ((allocation - used) <= 0)
        //     {
        //         return m;
                
        //     }
        //     else if ((((double) used / (double) allocation) > 0.4) && ((used / allocation ) <= 1))
        //     {
        //        if ((l->getX() == m->getX() && (l->getY() == m->getY()))) 
        //        {
        //            delete l;
        //            return m;
        //        }
        //     }
        
        l->setX(m->getX());
        l->setY(m->getY());
        for (int i = 0; i < 8; i++) {
            m->setX(i);
            for (int j = 0; j < 8; j++) {
                m->setY(j);
                if (board->checkMove(m, side)) {
                    
                    
                    Board br_board = Board();
                    br_board.copyFromBoard(board);
                    br_board.doMove(m, side);
                    
                    double path_value;
                    double alpha = -100;
                    double beta = 100;
                    
                    Side other = (side == BLACK) ? WHITE : BLACK;
                    if (this->mySide == side)
                    {
                        path_value = minimax(&br_board, depth,!isMax, other, 
                                             &alpha, &beta, msLeft, limit, isTest);
                    }
                    else //this is a hypothetical calculation for the opponent 
                    {
                        path_value = minimax(&br_board, depth + 1, !isMax, 
                                    other, &alpha, &beta, msLeft, limit, isTest);
                    }
                    if (isMax ^ (path_value < value))
                    {
                        value = path_value;
                        xcor = i;
                        ycor = j;
                    }
                    
            }
        }
                }
        if (depth == 0 && isMax)
        {
            m->setX(xcor);
            m->setY(ycor); 
        }
        if (xcor == -1)
            {
                return nullptr;
            }
            // auto end = std::chrono::system_clock::now();
            // std::chrono::duration<double> elapsed = end - start;
            // used = elapsed.count() * 1000; //is in microseconds
        }
        unordered_map<bitset<128>, double>::iterator it;
        return m;
}

/**
 * @brief Evaluation function
 */
double Player::evaluate(Board *board) {
    double corners_weight = 0.3 * evaluateCornerCloseness(board);
    double mobility_weight = 0.2 * evaluateMobility(board);
    double stability_weight = 0.25;
    double coins_weight = 0.25 * evaluateCoins(board);
    return corners_weight + mobility_weight + stability_weight + coins_weight;
    // return evaluateWeightedCoins(board);
}

double Player::evaluateCornerCloseness(Board *board) {
    int myTiles = 0;
    int oppTiles = 0;
    int myCorners = 0;
    int oppCorners = 0;
    int dx, dy;
    for (int x = 0; x < 8; x += 7) {
        for (int y = 0; y < 8; y += 7) {
            if(!board->occupied(x,y)) {
                (x == 0) ? dx = 1 : dx = 6;
                (y == 0) ? dy = 1 : dy = 6;
                if (board->get(this->mySide, x, dy)) {
                    myTiles++;
                }
                else if (board->get(this->oppSide, x, dy)) {
                    oppTiles++;
                }
                if (board->get(this->mySide, dx, y)) {
                    myTiles++;
                }
                else if (board->get(this->oppSide, dx, y)) {
                    oppTiles++; 
                }
            }
            else {
                if(board->get(this->mySide, x, y))
                    myCorners++;
                else
                    oppCorners++;
            }
        }
    }
    return 12.5 * (myCorners - oppCorners) - 6.25 * (myTiles - oppTiles);
}

double Player::evaluateMobility(Board *board) {
    double myCount = (double) board->countMoves(this->mySide);
    double oppCount = (double) board->countMoves(this->oppSide);

    if(myCount > oppCount)
        return (100 * myCount) / (myCount + oppCount);
    else if (myCount < oppCount)
        return (-100 * oppCount) / (myCount + oppCount);
    else
        return 0;
}

/**
 * @brief Counts difference in coins and normalizes value 
 * to the range [-100,100]
 */
double Player::evaluateCoins(Board *board) {
    double diff = board->count(this->mySide) - board->count(this->oppSide);
    return 100 * diff / 64;
}

/**
 * @brief Counts difference in coins weighting by position. 
 */
double Player::evaluateWeightedCoins(Board *board) {
    int myScore = 0;
    int oppScore = 0;
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if (board->get(this->mySide, x, y))
                myScore += getWeight(x, y);
            else if (board->get(this->oppSide, x, y))
                oppScore += getWeight(x, y);
        }
    }
    return double (myScore - oppScore);
}

/**
 * @brief Returns static weight of given coordinate
 */
double Player::getWeight(int x, int y) {
    double symMove = pow((x-3.5), 4) + pow((y-3.5), 4);

    // Attempt to optimize for time 
    if (symMove >= symMove11) {
        if (symMove == symMove20) {
            return 2;
        }
        else if (symMove > symMove20) {
            if (symMove == symMove10)
                return -3;
            else
                return 4;
        }
        else {
            if (symMove == symMove30)
                return 0;
            else
                return -4;
        }
    }
    else {
        if (symMove >= symMove31) {
            return -1;
        }
        else {
            if (symMove == symMove32)
                return 0;
            else
                return 1;
        }
    }
    // if (symMove == symMove00)
    //     return 4;
    // else if (symMove == symMove10)
    //     return -3;
    // else if (symMove == symMove11)
    //     return -4;
    // else if (symMove == symMove20)
    //     return 2;
    // else if (symMove == symMove21 || symMove == symMove31)
    //     return -1;
    // else if (symMove == symMove22 || symMove == symMove33)
    //     return 1;
    // else // (symMove == symMove32 || symMove == symMove30)
    //     return 0;

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
