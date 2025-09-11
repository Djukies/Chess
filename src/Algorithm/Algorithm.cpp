#include "Algorithm.h"



Algorithm::Algorithm(Board *board) : board(board) {

}

void Algorithm::calcBestMove() {
    this->bestMove = this->board->movesVector[0];
    int bestMoveEval = 0;
    for (Move move : this->board->movesVector) {
        makeMove(board, move);
        int moveEval = evaluatePosition();
        std::cout << intToX(move & startSquareMask) << "," << intToY(move & startSquareMask) << ", " << intToX((move & targetSquareMask) >> 6) << "," << intToY((move & targetSquareMask) >> 6) << ", " << ((move & targetSquareMask) >> 6) << ": " << moveEval << std::endl;
        if (moveEval > bestMoveEval) {
            this->bestMove = move;
            bestMoveEval = moveEval;
        }
        unMakeMove(board, move);
    }
}

int Algorithm::evaluatePosition() {
    int eval = 0;
    eval += std::popcount(board->blackPieces & board->pawns) * pieceValues[PAWN];
    eval += std::popcount(board->blackPieces & board->knights) * pieceValues[KNIGHT];
    eval += std::popcount(board->blackPieces & board->bishops) * pieceValues[BISHOP];
    eval += std::popcount(board->blackPieces & board->rooks) * pieceValues[ROOK];
    eval += std::popcount(board->blackPieces & board->queens) * pieceValues[QUEEN];

    eval -= std::popcount(board->whitePieces & board->pawns) * pieceValues[PAWN];
    eval -= std::popcount(board->whitePieces & board->knights) * pieceValues[KNIGHT];
    eval -= std::popcount(board->whitePieces & board->bishops) * pieceValues[BISHOP];
    eval -= std::popcount(board->whitePieces & board->rooks) * pieceValues[ROOK];
    eval -= std::popcount(board->whitePieces & board->queens) * pieceValues[QUEEN];

    return eval;
}
