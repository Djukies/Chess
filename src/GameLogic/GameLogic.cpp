#include "GameLogic.h"
#include "MoveGeneration/MoveGeneration.h"
#include <iostream>

GameLogic::GameLogic(Board *board) : board(board) {}

void GameLogic::undoMoves() {
    if (!IsKeyPressed(KEY_LEFT_SHIFT)) {
        if (IsKeyPressed(KEY_LEFT)) {

            if (board->fullMoves - firstMadMove <= 0) {
                return;
            }
            unMakeMove(board, madeMoves[board->fullMoves - firstMadMove - 1]);
            board->movesVector = calculateLegalMoves(board);
            board->movesMap = moveVectorToMap(board->movesVector);
        } else if (IsKeyPressed(KEY_RIGHT)) {
            if (board->fullMoves - firstMadMove < madeMoves.size()) {
                Move move = madeMoves[board->fullMoves - firstMadMove];
                makeMove(board, move);
                board->movesVector = calculateLegalMoves(board);
                board->movesMap = moveVectorToMap(board->movesVector);
            }
        }
    }
}


bool GameLogic::tryMove(uint8_t oldPos, uint8_t newPos) {
    Move move = getMove(oldPos, newPos, board->movesMap);
    if (move != 0) {
        makeMove(board, move);

        board->movesVector = calculateLegalMoves(board);
        board->movesMap = moveVectorToMap(board->movesVector);
        if (board->fullMoves-firstMadMove > madeMoves.size()) {
            madeMoves.push_back(move);
            return true;
        }

        if (move != madeMoves[board->fullMoves - firstMadMove - 1]) {
            uint size = madeMoves.size();
            for (int i = board->fullMoves - firstMadMove - 1; i < size; i++) {
                madeMoves.pop_back();
            }
            madeMoves.push_back(move);
        }
        return true;
    }
    return false;
}

std::map<integer, std::vector<Move>> GameLogic::moveVectorToMap(std::vector<Move> movesToRework) {
    std::map<integer, std::vector<Move>> movesToReturn;

    for (Move move : movesToRework) {
        movesToReturn[move & startSquareMask].push_back(move);
    }

    return movesToReturn;
}