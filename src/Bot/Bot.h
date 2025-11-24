#ifndef BOT_H
#define BOT_H

#include <functional>
#include <thread>

#include "../Types.h"
#include "Algorithm/Algorithm.h"
#include "../Board/Board.h"
#include "../GameLogic/GameLogic.h"


class Bot {
private:
    Board* board = new Board();
    Algorithm* algorithm = new Algorithm(board);
    GameLogic* gamelogic = new GameLogic(board, nullptr, nullptr);

    std::thread searchThread;


public:
    Bot();
    std::function<void(std::string)> sendToGUI;
    void setUpBoard(std::string FEN, std::vector<std::string> moves);

    void findBestMove(int depth);
};

#endif