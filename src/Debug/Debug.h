#ifndef CHESS_DEBUG_H
#define CHESS_DEBUG_H
#include <vector>
#include <raylib.h>
#include <chrono>
#include <iomanip>
#include "../GameLogic/BitboardUtility/BitboardUtility.h"
#include "../GameLogic/PrecomputedData/PrecomputedData.h"
#include "../GameLogic/MoveGeneration/MoveGeneration.h"
#include "../Types.h"
#include "../Renderer/Renderer.h"
#include "../Bot/Algorithm/Stockfish/Stockfish.h"


class Debug {
private:
    int toggleAllDebugsKey = KEY_B;
    int showNextBitboard = KEY_RIGHT;
    int showPreviousBitBoard = KEY_LEFT;
    Renderer* renderer;
    Board* board;
    GameLogic* gameLogic;
    void showBitBoards();
    int currentBitBoard = 0;

    int currentSquare = 0;
    int currentDir = 0;

public:
    Debug(Renderer* renderer, Board* board, GameLogic* gameLogic);
    std::string fen;
    void debugAll();
    void moveGenTest(int depth);
    std::vector<bit_board> bitBoards = {};
};


#endif //CHESS_DEBUG_H
