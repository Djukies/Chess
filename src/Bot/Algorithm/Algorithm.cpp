#include "Algorithm.h"

#include <algorithm>
#include <chrono>
#include <climits>

#include "../../GameLogic/MoveGeneration/MoveGeneration.h"


Algorithm::Algorithm(Board *board) : board(board) {

}

const int pawnSquareTable[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
   50, 50, 50, 50, 50, 50, 50, 50,
   10, 10, 20, 30, 30, 20, 10, 10,
    5,  5, 10, 25, 25, 10,  5,  5,
    0,  0,  0, 20, 20,  0,  0,  0,
    5, -5,-10,  0,  0,-10, -5,  5,
    5, 10, 10,-20,-20, 10, 10,  5,
    0,  0,  0,  0,  0,  0,  0,  0
};

const int knightSquareTable[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};

const int bishopSquareTable[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

const int rookSquareTable[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10, 10, 10, 10, 10,  5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
    0,  0,  0,  5,  5,  0,  0,  0
};

const int queenSquareTable[64] = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
     -5,  0,  5,  5,  5,  5,  0, -5,
      0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};

const int kingSquareTable[64] = {
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
     20, 20,  0,  0,  0,  0, 20, 20,
     20, 30, 10,  0,  0, 10, 30, 20
};

// Add similar tables for bishops, rooks, queens, and kings

int evaluatePosition(const Board* board, int depth) {
    if (board->checkMate) {
        if (board->checkMateWhite) return 1000 + depth;
        else return -1000 - depth;
    }
    if (board->staleMate) {
        return 0;
    }

    int eval = 0;

    // Evaluate white pieces
    uint64_t pieces = board->whitePieces & board->pawns;
    while (pieces) {
        int square = __builtin_ctzll(pieces); // Get least significant bit position
        eval += pieceValues[PAWN] + pawnSquareTable[square];
        pieces &= pieces - 1; // Clear the least significant bit
    }

    pieces = board->whitePieces & board->knights;
    while (pieces) {
        int square = __builtin_ctzll(pieces);
        eval += pieceValues[KNIGHT] + knightSquareTable[square];
        pieces &= pieces - 1;
    }

    pieces = board->whitePieces & board->bishops;
    while (pieces) {
        int square = __builtin_ctzll(pieces);
        eval += pieceValues[BISHOP] + bishopSquareTable[square];
        pieces &= pieces - 1;
    }

    pieces = board->whitePieces & board->rooks;
    while (pieces) {
        int square = __builtin_ctzll(pieces);
        eval += pieceValues[ROOK] + rookSquareTable[square];
        pieces &= pieces - 1;
    }

    pieces = board->whitePieces & board->queens;
    while (pieces) {
        int square = __builtin_ctzll(pieces);
        eval += pieceValues[QUEEN] + queenSquareTable[square];
        pieces &= pieces - 1;
    }

    pieces = board->whitePieces & board->kings;
    while (pieces) {
        int square = __builtin_ctzll(pieces);
        eval += pieceValues[KING] + kingSquareTable[square];
        pieces &= pieces - 1;
    }




    // Evaluate black pieces (mirror the square index)
    pieces = board->blackPieces & board->pawns;
    while (pieces) {
        int square = __builtin_ctzll(pieces);
        eval -= pieceValues[PAWN] + pawnSquareTable[square ^ 56]; // XOR 56 flips rank
        pieces &= pieces - 1;
    }

    pieces = board->blackPieces & board->knights;
    while (pieces) {
        int square = __builtin_ctzll(pieces);
        eval -= pieceValues[KNIGHT] + knightSquareTable[square ^ 56];
        pieces &= pieces - 1;
    }

    pieces = board->blackPieces & board->bishops;
    while (pieces) {
        int square = __builtin_ctzll(pieces);
        eval -= pieceValues[BISHOP] + bishopSquareTable[square ^ 56];
        pieces &= pieces - 1;
    }

    pieces = board->blackPieces & board->rooks;
    while (pieces) {
        int square = __builtin_ctzll(pieces);
        eval -= pieceValues[ROOK] + rookSquareTable[square ^ 56];
        pieces &= pieces - 1;
    }

    pieces = board->blackPieces & board->queens;
    while (pieces) {
        int square = __builtin_ctzll(pieces);
        eval -= pieceValues[QUEEN] + queenSquareTable[square ^ 56];
        pieces &= pieces - 1;
    }

    pieces = board->blackPieces & board->kings;
    while (pieces) {
        int square = __builtin_ctzll(pieces);
        eval -= pieceValues[KING] + kingSquareTable[square ^ 56];
        pieces &= pieces - 1;
    }

    return eval;
}

/*int quiescence(Board* board, int alpha, int beta) {
    int standPat = evaluatePosition(board, 0);

    /*if (standPat >= beta) {
        return beta;
    }
    if (alpha < standPat) {
        alpha = standPat;
    }#1#

    std::vector<Move> moves = calculateLegalMoves(board);
    std::vector<Move> captures;

    // Filter only capture moves
    for (Move move : moves) {
        int targetSquare = (move & targetSquareMask) >> 6;
        if (containsSquare(board->whitePieces | board->blackPieces, targetSquare)) {
            captures.push_back(move);
        }
    }

    // Sort captures by captured piece value (MVV-LVA: Most Valuable Victim - Least Valuable Attacker)
    std::ranges::sort(captures, [&](Move a, Move b) {
        return pieceValues[getPiece(board, getStartsquare(a))] > pieceValues[getPiece(board, getStartsquare(b))];
    });

    if (captures.empty()) return standPat;

    Move capture = captures.front();
    Board newBoard = *board;
    makeMove(&newBoard, capture);

    int score = -quiescence(&newBoard, -beta, -alpha);

    if (score >= standPat) {
        standPat = score;
    }
    /*if (score >= beta) {
        return beta;
    }
    if (score > alpha) {
        alpha = score;
    }#1#

    return standPat;
}*/

double movesSearched = 0;

int miniMax(Board* board, int depth, int alpha, int beta, bool maximizingPlayer) {
    std::vector<Move> moves = calculateLegalMoves(board);

    // Base case: depth 0 OR no legal moves (checkmate/stalemate)
    if (depth == 0 || moves.empty()) {
        movesSearched++;
        return evaluatePosition(board, depth);
        //return quiescence(board, alpha, beta);
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
    */
    std::cout << "Best Move For " << (board->whiteToMove ? "White" : "Black") << ": " << bestEval << ", " << intToString(this->bestMove & startSquareMask) << intToString((this->bestMove & targetSquareMask) >> 6) << ", Time Taken: " << time.count() << " ms, Moves Seached: " << movesSearched << std::endl;

    return this->bestMove;
}


