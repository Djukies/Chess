#ifndef CHESS_ZOBRIST_H
#define CHESS_ZOBRIST_H

#pragma once
#include <cstdint>
#include <array>
#include <random>

constexpr int NUM_PIECE_TYPES = 12;  // white & black: pawn..king
constexpr int NUM_SQUARES = 64;
constexpr int NUM_CASTLING_RIGHTS = 4; // K, Q, k, q
constexpr int NUM_FILES = 8;

struct Zobrist {
    static std::array<std::array<uint64_t, NUM_SQUARES>, NUM_PIECE_TYPES> pieceSquare;
    static std::array<uint64_t, NUM_CASTLING_RIGHTS> castling;
    static std::array<uint64_t, NUM_FILES> enPassantFile;
    static uint64_t sideToMove;

    static void init();
};

#endif //CHESS_ZOBRIST_H