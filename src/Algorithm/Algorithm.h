#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "../Types.h"
#include "../Board/Board.h"



class Algorithm {
    private:
    Board* board;
    public:
    explicit Algorithm(Board* board);
    Move bestMove{};
    void calcBestMove();
    int evaluatePosition();
};



#endif //ALGORITHM_H
