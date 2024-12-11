#ifndef CHESS_DEBUG_H
#define CHESS_DEBUG_H
#include <vector>
#include <raylib.h>
#include "../Types.h"
#include "../Renderer/Renderer.h"


class Debug {
private:
    int toggleAllDebugsKey = KEY_B;
    int showNextBitboard = KEY_RIGHT;
    int showPreviousBitBoard = KEY_LEFT;
    Renderer* renderer;
    Board* board;
    void showBitBoards();
    int currentBitBoard = 0;
public:
    Debug(Renderer* renderer, Board* board);
    void debugAll();
    std::vector<bit_board> bitBoards = {};
};


#endif //CHESS_DEBUG_H
