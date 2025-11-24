#include "Algorithm.h"

#include <algorithm>
#include <chrono>
#include <climits>

#include "../../GameLogic/MoveGeneration/MoveGeneration.h"


Algorithm::Algorithm(Board *board) : board(board) {

}

int evaluatePosition(const Board* board, int depth) {
    if (board->checkMate) {
        if (board->checkMateWhite) return 1000 + depth;
        else return -1000 - depth;
    }
    if (board->staleMate) {
        return 0;
    }
    // CAPTURE EVERYTHING THEN EVAL
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

double movesSearched = 0;

int miniMax(Board* board, int depth, int alpha, int beta, bool maximizingPlayer) {
    std::vector<Move> moves = calculateLegalMoves(board);

    // Base case: depth 0 OR no legal moves (checkmate/stalemate)
    if (depth == 0 || moves.empty()) {
        movesSearched++;
        return evaluatePosition(board, depth);
    }

    int maxEval = maximizingPlayer ? INT_MIN : INT_MAX; // Assign the worst possible move: for white it's negative infinity for black positive infinity
    for (const Move& move : moves) { // Loop over all moves currently at
        makeMove(board, move); // Play move
        // Go recursive till depth = 0, alpha, beta stay the same, at deeper depth it will try to maximize the other player (black => white, white => black)
        int eval = miniMax(board, depth - 1, alpha, beta, !maximizingPlayer);

        unMakeMove(board, move); // Unplay the move

        // If maximizing white....
        if (maximizingPlayer) {
            maxEval = std::max(eval, maxEval); // Max eval should become as high as possible
            alpha = std::max(alpha, eval); // Alpha should become as high as possible
        }
        else { // Else if maximizing black ...
            maxEval = std::min(eval, maxEval); // Max eval should become as low as possible
            beta = std::min(beta, eval); // Beta should become as low as possible
        }
        if (beta <= alpha) {break;}
    }
    return maxEval;
}

void Algorithm::runMinimax(bool engineIsWhite) {
    std::vector<Move> moves = calculateLegalMoves(board);

    int bestEval = engineIsWhite ? INT_MIN : INT_MAX;

    for (const Move& move : moves) {
        makeMove(board, move);
        int eval = miniMax(board, searchDepth - 1, INT_MIN, INT_MAX, !engineIsWhite);
        unMakeMove(board, move);
        bestMovesMap.emplace_back(eval, move);
        if (engineIsWhite) {
            if (eval > bestEval) {
                bestEval = eval;
                bestMove = move;
            }
            //alpha = std::max(alpha, eval);
        } else {
            if (eval < bestEval) {
                bestEval = eval;
                bestMove = move;
            }
            //beta = std::min(beta, eval);
        }
    }
}

Move Algorithm::calcBestMove(int depth, bool engineIsWhite) {
    std::cout << "ALGORITHM: WHITETOMOVE: " << engineIsWhite << std::endl;
    if (board->checkMate || board->staleMate) {
        return 0;
    }
    movesSearched = 0;
    bestMovesMap.clear();
    this->searchDepth = depth;

    const auto start = std::chrono::high_resolution_clock::now();

    runMinimax(engineIsWhite);

    const auto end = std::chrono::high_resolution_clock::now();
    const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::ranges::sort(bestMovesMap,
                      [engineIsWhite](const auto& a, const auto& b) {
                          return engineIsWhite ? a.first > b.first : a.first < b.first; // highest eval first
                      });

    int bestEval = bestMovesMap.front().first;
    this->bestMove = bestMovesMap.front().second;


    /*for (auto [eval, move] : bestMovesMap) {
        std::cout << "Move: " << eval << ", " << intToString(move & startSquareMask) << intToString((move & targetSquareMask) >> 6) << std::endl;
    }
    std::cout << "Best Move For " << (board->whiteToMove ? "White" : "Black") << ": " << bestEval << ", " << intToString(this->bestMove & startSquareMask) << intToString((this->bestMove & targetSquareMask) >> 6) << ", Time Taken: " << time.count() << " ms, Moves Seached: " << movesSearched << std::endl;
    */
    return this->bestMove;
}


