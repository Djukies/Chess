#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "../Types.h"
#include "../Board/Board.h"
#include "../Algorithm/Algorithm.h"



class Algorithm {
    private:
    Board* board;
    int miniMax(int depth, bool maximizingPlayer);
    int searchDepth;
    public:
    explicit Algorithm(Board* board);
    Move bestMove{};
    void calcBestMove(int depth, bool engineIsWhite);
    int evaluatePosition();
};



#endif //ALGORITHM_H
