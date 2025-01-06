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
            unMakeSmallMove(board, madeMoves[board->fullMoves - firstMadMove - 1]);
            board->movesVector = calculateLegalMoves(board);
            board->movesMap = moveVectorToMap(board->movesVector);
        } else if (IsKeyPressed(KEY_RIGHT)) {
            if (board->fullMoves - firstMadMove < madeMoves.size()) {
                Move Move = madeMoves[board->fullMoves - firstMadMove];
                makeSmallMove(board, Move);
                board->movesVector = calculateLegalMoves(board);
                board->movesMap = moveVectorToMap(board->movesVector);
            }
        }
    }
}


bool GameLogic::tryMove(uint8_t oldPos, uint8_t newPos) {
    Move smallMove = getSmallMove(oldPos, newPos, board->movesMap);
    if (smallMove != 0) {
        makeSmallMove(board, smallMove);

        board->movesVector = calculateLegalMoves(board);
        board->movesMap = moveVectorToMap(board->movesVector);
        if (board->fullMoves-firstMadMove > madeMoves.size()) {
            madeMoves.push_back(smallMove);
            return true;
        }

        if (smallMove != madeMoves[board->fullMoves - firstMadMove - 1]) {
            uint size = madeMoves.size();
            for (int i = board->fullMoves - firstMadMove - 1; i < size; i++) {
                madeMoves.pop_back();
            }
            madeMoves.push_back(smallMove);
        }
        return true;
    }
    return false;
}

std::map<integer, std::vector<Move>> GameLogic::moveVectorToMap(std::vector<Move> movesToRework) {
    std::map<integer, std::vector<Move>> movesToReturn;

    for (Move Move : movesToRework) {
        movesToReturn[Move & startSquareMask].push_back(Move);
    }

    return movesToReturn;
}