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