#include <string>
#include "iostream"
#include "../Types.h"
#include "../Setup/Setup.h"
#include "../GameLogic/Bitboard Utility/BitboardUtility.h"
#include "raylib.h"
#include <vector>


#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H


Board* loadBoardFromFEN(const std::string fen);

void makeSmallMove(Board* board, Move Move);
void unMakeSmallMove(Board* board, Move Move);

#endif //CHESS_BOARD_H
