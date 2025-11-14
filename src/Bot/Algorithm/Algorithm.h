#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "../../Types.h"
#include "../../Board/Board.h"
#include "../Algorithm/Algorithm.h"

#include <random>

int evaluatePosition(const Board* board, int depth);

class Algorithm {

private:


    Board* board;
    //int miniMax(int depth, int alpha, int beta, bool maximizingPlayer);
    int searchDepth;
    void runMinimax(bool engineIsWhite);
public:
    explicit Algorithm(Board* board);
    Move bestMove{};
    void calcBestMove(int depth, bool engineIsWhite);
    //int evaluatePosition(int depth);
    std::vector<std::pair<int, Move>> bestMovesMap = {};
};



#endif //ALGORITHM_H
