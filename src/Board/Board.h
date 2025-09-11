#include <string>
#include "iostream"
#include "../Types.h"
#include "../Setup/Setup.h"
#include "../GameLogic/BitboardUtility/BitboardUtility.h"
#include "raylib.h"
#include <vector>


#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H


Board* loadBoardFromFEN(const std::string fen);

void makeMove(Board* board, Move move);
void unMakeMove(Board* board, Move move);

#endif //CHESS_BOARD_H
