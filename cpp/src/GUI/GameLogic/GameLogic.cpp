#include "GameLogic.h"
#include <iostream>

GameLogic::GameLogic(Board *board) {
    Zobrist::init();
    board->zobristKey = computeHash(board);
    board->firstMadMove = board->fullMoves;
}







void GameLogic::undoMoves(Board* board) {
    if (!IsKeyPressed(KEY_LEFT_SHIFT)) {
        if (IsKeyPressed(KEY_LEFT)) {

            if (board->fullMoves - board->firstMadMove <= 0) {
                return;
            }
            unMakeMove(board, board->madeMoves[board->fullMoves - board->firstMadMove - 1]);
            board->movesVector = calculateLegalMoves(board);
            board->movesMap = moveVectorToMap(board->movesVector);
        } else if (IsKeyPressed(KEY_RIGHT)) {
            if (board->fullMoves - board->firstMadMove < (int)board->madeMoves.size()) {
                Move move = board->madeMoves[board->fullMoves - board->firstMadMove];
                makeMove(board, move);
                board->movesVector = calculateLegalMoves(board);
                board->movesMap = moveVectorToMap(board->movesVector);
            }
        }
    }
}





/*
void GameLogic::letAlgoMakeMove(Board* board) {
    if (board->whiteToMove) {
        if (algoWhiteStockfish) {
            letAlgoStockfishMakeMove(board);
            return;
        }
        if (algorithmWhite->bestMove == 0) {
            algorithmWhite->calcBestMove(4, TEAMWHITE);
        }
    } else {
        if (algoBlackStockfish) {
            letAlgoStockfishMakeMove(board);
            return;
        }
        if (algorithmBlack->bestMove == 0) {
            algorithmBlack->calcBestMove(4, TEAMBLACK);
        }
    }
    makeMove(board, board->whiteToMove ? algorithmWhite->bestMove : algorithmBlack->bestMove);
    board->movesVector = calculateLegalMoves(board);
    board->movesMap = moveVectorToMap(board->movesVector);
    Move move = board->whiteToMove ? algorithmWhite->bestMove : algorithmBlack->bestMove;
    if (board->fullMoves-board->firstMadMove > board->madeMoves.size()) {
        board->madeMoves.push_back(move);
        if (board->whiteToMove) {
            algorithmWhite->calcBestMove(4, TEAMWHITE);
        } else {
            algorithmBlack->calcBestMove(4, TEAMBLACK);
        }
        calculateLegalMoves(board);
        if (board->checkMate || board->staleMate) {
            std::cout << "GAME ENDED" << std::endl;

            return;
        }
        return;
    }

    if (move != board->madeMoves[board->fullMoves - board->firstMadMove - 1]) {
        uint size = board->madeMoves.size();
        for (int i = board->fullMoves - board->firstMadMove - 1; i < size; i++) {
            board->madeMoves.pop_back();
        }
        board->madeMoves.push_back(move);
    }
    if (board->whiteToMove) {
        if (algoWhiteStockfish) return;
        algorithmWhite->calcBestMove(4, TEAMWHITE);
    } else {
        if (algoBlackStockfish) return;
        algorithmBlack->calcBestMove(4, TEAMBLACK);
    }
}
*/

/*void GameLogic::letAlgoStockfishMakeMove(Board* board) {
    std::string move = StockfishGetMove(board->movesMade, board->startingFen);
    integer from = charToInt(move.substr(0, 2));
    integer to = charToInt(move.substr(2, 2));
    std::cout << tryMove(board, from, to) << std::endl;
}*/



