#ifndef CHESS_PRECOMPUTEDDATA_H
#define CHESS_PRECOMPUTEDDATA_H

#include "../../Types.h"

inline integer distanceToEdge[64][8];

// Function to compute values
inline void computeDistanceToEdge() {
    for (integer pos = 0; pos < 64; ++pos) {
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

inline void precompute() {
    computeDistanceToEdge();
}

#endif //CHESS_PRECOMPUTEDDATA_H
