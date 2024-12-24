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

void makeMove(Board* board, Move move);
void makeSmallMove(Board* board, small_move move);
void unMakeSmallMove(Board* board, small_move move);
void unmakeMove(Board* board, Move move);

#endif //CHESS_BOARD_H
