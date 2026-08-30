#include "Zobrist.h"



std::array<std::array<uint64_t, NUM_SQUARES>, NUM_PIECE_TYPES> Zobrist::pieceSquare;
std::array<uint64_t, NUM_CASTLING_RIGHTS> Zobrist::castling;
std::array<uint64_t, NUM_FILES> Zobrist::enPassantFile;
uint64_t Zobrist::sideToMove;

void Zobrist::init() {
    std::mt19937_64 rng(0xABCDEF123456789ULL); // fixed seed for reproducibility
    std::uniform_int_distribution<uint64_t> dist;

    for (auto &pieceArr : pieceSquare)
        for (auto &sq : pieceArr)
            sq = dist(rng);

    for (auto &c : castling)
        c = dist(rng);

    for (auto &e : enPassantFile)
        e = dist(rng);

    sideToMove = dist(rng);
}

uint64_t computeHash(Board* board) {
    uint64_t hash = 0;

    // For each piece on board (example)
    for (int sq = 0; sq < 64; ++sq) {
        int piece = getPiece(board, sq) + (containsSquare(board->whitePieces, sq) ? 0 : 5); // e.g. 0-11 or -1 if empty
        if (piece != 12)
            hash ^= Zobrist::pieceSquare[piece][sq];
    }

    // Side to move
    if (board->whiteToMove)
        hash ^= Zobrist::sideToMove;

    // Castling rights
    if (containsSquare(board->castleRights, 0)) hash ^= Zobrist::castling[0];
    if (containsSquare(board->castleRights, 1)) hash ^= Zobrist::castling[1];
    if (containsSquare(board->castleRights, 2)) hash ^= Zobrist::castling[2];
    if (containsSquare(board->castleRights, 3)) hash ^= Zobrist::castling[3];

    // En
    if (board->enPassantSquare != 0) {
        int epFile = intToX(board->enPassantSquare); // 0..7 or -1
        hash ^= Zobrist::enPassantFile[epFile];
    }
    std::cout << hash << std::endl;
    return hash;
}