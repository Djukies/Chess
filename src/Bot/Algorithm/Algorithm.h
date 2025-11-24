#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <atomic>

#include "../../Types.h"
#include "../../Board/Board.h"
#include "../Algorithm/Algorithm.h"

#include <random>

int evaluatePosition(const Board* board, int depth);

class Algorithm {

private:
    //int miniMax(int depth, int alpha, int beta, bool maximizingPlayer);
    int searchDepth;
    void runMinimax(bool engineIsWhite);
public:
    Board* board;
    explicit Algorithm(Board* board);
    Move bestMove{};
    Move calcBestMove(int depth, bool engineIsWhite);
    //int evaluatePosition(int depth);
    std::vector<std::pair<int, Move>> bestMovesMap = {};
    std::atomic<bool> stopFlag{false};
};



#endif //ALGORITHM_H
