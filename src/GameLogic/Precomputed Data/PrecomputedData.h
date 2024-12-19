#ifndef CHESS_PRECOMPUTEDDATA_H
#define CHESS_PRECOMPUTEDDATA_H

#include <vector>
#include "../../Types.h"
#include "../Bitboard Utility/BitboardUtility.h"

inline integer distanceToEdge[64][8];
inline int directions[8] = {-8, 8, -1, 1, -9, 9, -7, 7};
inline bit_board possibleKingMoves[64];
inline bit_board possibleKnightMoves[64];

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

        possibleKingMoves[pos] = (kingYM1|kingY|kingYP1) & (kingXM1|kingX|kingXP1) & ~(1ULL << pos);
    }
}

inline void computeKnightMoves() {
    int dirs[8] = {-17, -15, -10, -6, 6, 10, 15, 17};
    for (integer pos = 0; pos < 64; pos++) {
        for (int dir: dirs) {
            possibleKnightMoves[pos] |= shift(1ULL, pos + dir);
        }
        if (containsSquare(rightSide, pos)) {
            possibleKnightMoves[pos] &= notFileAB;
        } else {
            possibleKnightMoves[pos] &= notFileGH;
        }
        if (containsSquare(downSide, pos)) {
            possibleKnightMoves[pos] &= notRank12;
        }
    }
}

inline void precompute() {
    computeDistanceToEdge();
    computeKingMoves();
    computeKnightMoves();
}

#endif //CHESS_PRECOMPUTEDDATA_H
