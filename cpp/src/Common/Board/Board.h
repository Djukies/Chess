#include <string>
#include "iostream"
#include "../Types.h"
#include "../Setup/Setup.h"
#include "../../Common/BitboardUtility/BitboardUtility.h"
#include "Zobrist.h"
#include "raylib.h"
#include <vector>


#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H


Board* loadBoardFromFEN(const std::string fen);

MadeMove makeMove(Board* board, Move move);
void unMakeMove(Board* board, Move move);

inline std::map<integer, std::vector<Move>> moveVectorToMap(const std::vector<Move>& movesToRework) {
    std::map<integer, std::vector<Move>> movesToReturn;

    for (Move move : movesToRework) {
        movesToReturn[move & startSquareMask].push_back(move);
    }

    return movesToReturn;
}

Move getMove(integer oldPos, integer newPos, std::map<integer, std::vector<Move>> moves);
bool isThreeFoldRepetition(Board* board);
bool tryMove(Board* board, uint8_t oldPos, uint8_t newPos);


#endif //CHESS_BOARD_H
