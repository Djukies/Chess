#ifndef BOT_H
#define BOT_H

#include <functional>
#include <thread>

#include "../../Common/Types.h"
#include "../Algorithm/Algorithm.h"
#include "../../Common/Board/Board.h"

#include "../../Common/MoveGeneration/MoveGeneration.h"


class Bot {
private:
    Board* board = new Board();
    Algorithm* algorithm = new Algorithm(board);
    std::thread searchThread;


public:
    Bot();
    std::function<void(std::string)> sendToGUI;
    void setUpBoard(std::string FEN, std::vector<std::string> moves);

    void findBestMove(int depth);
};

#endif