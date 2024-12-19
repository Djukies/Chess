#ifndef CHESS_PRECOMPUTEDDATA_H
#define CHESS_PRECOMPUTEDDATA_H

#include "../../Types.h"
#include "../Bitboard Utility/BitboardUtility.h"

inline integer distanceToEdge[64][8];
inline bit_board kingMoves[64];

// Function to compute values
inline void computeDistanceToEdge() {
    for (integer pos = 0; pos < 64; pos++) {
        integer up = intToY(pos);
        integer down = 7 - up;
        integer left = intToX(pos);
        integer right = 7 - left;

        distanceToEdge[pos][0] = up;
        distanceToEdge[pos][1] = down;
        distanceToEdge[pos][2] = left;
        distanceToEdge[pos][3] = right;
        distanceToEdge[pos][4] = std::min(up, left);
        distanceToEdge[pos][5] = std::min(down, right);
        distanceToEdge[pos][6] = std::min(up, right);
        distanceToEdge[pos][7] = std::min(down, left);
    }
}

inline void computeKingMoves() {
    for (integer pos = 0; pos < 64; pos++) {
        int x = intToX(pos);
        int y = intToY(pos);

        bit_board kingY = (uint64_t )0xFF << (y*8);
        bit_board kingX = 0x0101010101010101ULL << x;

        bit_board kingYP1 = kingY << 8;
        bit_board kingYM1 = kingY >> 8;

        bit_board kingXP1 = (kingX << 1) & notFileA;
        bit_board kingXM1 = (kingX >> 1) & notFileH;

        kingMoves[pos] = (kingYM1|kingY|kingYP1) & (kingXM1|kingX|kingXP1) & ~(1ULL << pos);
    }
}

inline void precompute() {
    computeDistanceToEdge();
    computeKingMoves();
}

#endif //CHESS_PRECOMPUTEDDATA_H
