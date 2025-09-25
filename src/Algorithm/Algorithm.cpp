#include "Algorithm.h"

#include <climits>

#include "../GameLogic/MoveGeneration/MoveGeneration.h"


Algorithm::Algorithm(Board *board) : board(board) {

}

int Algorithm::miniMax(int depth, bool maximizingPlayer) {
    if (depth == 0) {
        return evaluatePosition();
    }

    std::vector<Move> moves = calculateLegalMoves(board);
    if (moves.empty()) {
        // No legal moves: return evaluation (e.g. mate/stalemate handled inside evaluate)
        return evaluatePosition();
    }

    if (maximizingPlayer) {
        int maxEval = INT_MIN;
        for (const Move& move : moves) {
            makeMove(board, move);
            int eval = miniMax(depth - 1, false);
            unMakeMove(board, move);
            if (eval > maxEval) maxEval = eval;
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (const Move& move : moves) {
            makeMove(board, move);
            int eval = miniMax(depth - 1, true);
            unMakeMove(board, move);
            if (eval < minEval) minEval = eval;
        }
        return minEval;
    }
}

void Algorithm::calcBestMove(int depth, bool engineIsWhite) {
    this->bestMove = this->board->movesVector[0];
    this->searchDepth = depth;
    //int bestMoveEval = miniMax(depth, INT_MIN, INT_MAX, true);

    std::vector<Move> moves = calculateLegalMoves(board);
    int bestEval = engineIsWhite ? INT_MIN : INT_MAX;

    for (const Move& move : moves) {
        makeMove(board, move);
        int eval = miniMax(searchDepth - 1, !engineIsWhite);
        unMakeMove(board, move);

        if (engineIsWhite) {
            if (eval > bestEval) {
                bestEval = eval;
                bestMove = move;
            }
        } else {
            if (eval < bestEval) {
                bestEval = eval;
                bestMove = move;
            }
        }
    }

    //std::cout << "Best Move: " << bestEval << ", " << intToX(this->bestMove & startSquareMask) << "," << intToY(this->bestMove & startSquareMask) << ", " << intToX((this->bestMove & targetSquareMask) >> 6) << "," << intToY((this->bestMove & targetSquareMask) >> 6) << std::endl;

}

int Algorithm::evaluatePosition() {
    if (board->checkMate)
    int eval = 0;
    eval += std::popcount(board->whitePieces & board->pawns) * pieceValues[PAWN];
    eval += std::popcount(board->whitePieces & board->knights) * pieceValues[KNIGHT];
    eval += std::popcount(board->whitePieces & board->bishops) * pieceValues[BISHOP];
    eval += std::popcount(board->whitePieces & board->rooks) * pieceValues[ROOK];
    eval += std::popcount(board->whitePieces & board->queens) * pieceValues[QUEEN];

    eval -= std::popcount(board->blackPieces & board->pawns) * pieceValues[PAWN];
    eval -= std::popcount(board->blackPieces & board->knights) * pieceValues[KNIGHT];
    eval -= std::popcount(board->blackPieces & board->bishops) * pieceValues[BISHOP];
    eval -= std::popcount(board->blackPieces & board->rooks) * pieceValues[ROOK];
    eval -= std::popcount(board->blackPieces & board->queens) * pieceValues[QUEEN];

    return eval;
}
