#ifndef CHESS_GAMELOGIC_H
#define CHESS_GAMELOGIC_H

#include "../../Common/Types.h"
#include "../../Common/Board/Board.h"
#include "../../Common/Board/Zobrist.h"
#include <map>
#include <vector>
#include <cmath>
#include <chrono>

#include "../../Engine/Algorithm/Algorithm.h"
#include "../../Engine/Algorithm/Stockfish/Stockfish.h"
#include "../../Common/MoveGeneration/MoveGeneration.h"




class GameLogic {
private:
    //Board* board;




public:
    explicit GameLogic(Board* board);
    void undoMoves(Board* board);
    bool algoWhiteStockfish = false;
    bool algoBlackStockfish = false;
    //void letAlgoMakeMove(Board* board);
    //void letAlgoStockfishMakeMove(Board* board);
};


#endif //CHESS_GAMELOGIC_H
