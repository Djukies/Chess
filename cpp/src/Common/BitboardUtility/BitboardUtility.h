#pragma once
#include "../Types.h"

#ifndef CHESS_BITBOARDUTILITY_H
#define CHESS_BITBOARDUTILITY_H

const bitboard fileA = 0x0101010101010101;
const bitboard fileB = fileA << 1;
const bitboard fileC = fileB << 1;
const bitboard fileD = fileC << 1;
const bitboard fileE = fileD << 1;
const bitboard fileF = fileE << 1;
const bitboard fileG = fileF << 1;
const bitboard fileH = fileG << 1;

const bitboard leftSide = fileA | fileB | fileC | fileD;
const bitboard rightSide = ~leftSide;

const bitboard notFileAB = ~(fileA|fileB);
const bitboard notFileGH = ~(fileG|fileH);
const bitboard notFileA = ~fileA;
const bitboard notFileH = ~fileH;

const bitboard rank1 = 0xFF;
const bitboard rank2 = rank1 << 8;
const bitboard rank3 = rank2 << 8;
const bitboard rank4 = rank3 << 8;
const bitboard rank5 = rank4 << 8;
const bitboard rank6 = rank5 << 8;
const bitboard rank7 = rank6 << 8;
const bitboard rank8 = rank7 << 8;

const bitboard upSide = rank1 | rank2 | rank3 | rank4;
const bitboard downSide = ~upSide;

const bitboard notRank12 = ~(rank1 | rank2);
const bitboard notRank78 = ~(rank7 | rank8);

inline bitboard shift(bitboard bitboard, int amount) {
    if (amount > 0) {
        return bitboard << amount;
    }
    return bitboard >> -amount;
}

inline int getIndex(bitboard bitboard) {
    return __builtin_ctzll(bitboard);
}

inline bool containsSquare(bitboard bitboard, integer square)
{
    return ((bitboard >> square) & 1) != 0;
}

inline Piece getPiece(Board* board, integer square) {
    if (containsSquare(board->pawns, square)) return PAWN;
    if (containsSquare(board->bishops, square)) return BISHOP;
    if (containsSquare(board->knights, square)) return KNIGHT;
    if (containsSquare(board->rooks, square)) return ROOK;
    if (containsSquare(board->queens, square)) return QUEEN;
    if (containsSquare(board->kings, square)) return KING;
    return NONE;
}


#endif //CHESS_BITBOARDUTILITY_H
