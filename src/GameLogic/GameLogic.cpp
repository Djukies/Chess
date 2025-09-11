#include "GameLogic.h"
#include "MoveGeneration/MoveGeneration.h"
#include <iostream>

GameLogic::GameLogic(Board *board, Algorithm* algorithm) : board(board), algorithm(algorithm) {}

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
        if ((move & flagMask) >> 12 >= 4) {
            std::cout << "PROMOTING" << std::endl;
        }
        makeMove(board, move);

        board->movesVector = calculateLegalMoves(board);
        board->movesMap = moveVectorToMap(board->movesVector);
        if (board->fullMoves-firstMadMove > madeMoves.size()) {
            madeMoves.push_back(move);
            if (board->algorithm && board->whiteToMove == board->algorithmIsWhite) {
                this->letAlgoMakeMove();
            }
            return true;
        }

        if (move != madeMoves[board->fullMoves - firstMadMove - 1]) {
            uint size = madeMoves.size();
            for (int i = board->fullMoves - firstMadMove - 1; i < size; i++) {
                madeMoves.pop_back();
            }
            madeMoves.push_back(move);
        }
        if (board->algorithm && board->whiteToMove == board->algorithmIsWhite) {
            this->letAlgoMakeMove();
        }
        return true;
    }
    return false;
}

void GameLogic::letAlgoMakeMove() {
    std::cout << "OOOHHHH" << std::endl;
    algorithm->calcBestMove();
    makeMove(board, algorithm->bestMove);
    board->movesVector = calculateLegalMoves(board);
    board->movesMap = moveVectorToMap(board->movesVector);
    Move move = algorithm->bestMove;
    if (board->fullMoves-firstMadMove > madeMoves.size()) {
        madeMoves.push_back(move);
        if (board->algorithm && board->whiteToMove == board->algorithmIsWhite) {
            this->letAlgoMakeMove();
        }
        return;
    }

    if (move != madeMoves[board->fullMoves - firstMadMove - 1]) {
        uint size = madeMoves.size();
        for (int i = board->fullMoves - firstMadMove - 1; i < size; i++) {
            madeMoves.pop_back();
        }
        madeMoves.push_back(move);
    }
    if (board->algorithm && board->whiteToMove == board->algorithmIsWhite) {
        this->letAlgoMakeMove();
    }
}


std::map<integer, std::vector<Move>> GameLogic::moveVectorToMap(std::vector<Move> movesToRework) {
    std::map<integer, std::vector<Move>> movesToReturn;

    for (Move move : movesToRework) {
        movesToReturn[move & startSquareMask].push_back(move);
    }

    return movesToReturn;
}