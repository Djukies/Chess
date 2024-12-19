#include "../../Types.h"

#ifndef CHESS_BITBOARDUTILITY_H
#define CHESS_BITBOARDUTILITY_H

const bit_board fileA = 0x0101010101010101;
const bit_board fileB = fileA << 1;
const bit_board fileC = fileB << 1;
const bit_board fileD = fileC << 1;
const bit_board fileE = fileD << 1;
const bit_board fileF = fileE << 1;
const bit_board fileG = fileF << 1;
const bit_board fileH = fileG << 1;

const bit_board leftSide = fileA | fileB | fileC | fileD;
const bit_board rightSide = ~leftSide;

const bit_board notFileAB = ~(fileA|fileB);
const bit_board notFileGH = ~(fileG|fileH);
const bit_board notFileA = ~fileA;
const bit_board notFileH = ~fileH;

const bit_board rank1 = 0xFF;
const bit_board rank2 = rank1 << 8;
const bit_board rank3 = rank2 << 8;
const bit_board rank4 = rank3 << 8;
const bit_board rank5 = rank4 << 8;
const bit_board rank6 = rank5 << 8;
const bit_board rank7 = rank6 << 8;
const bit_board rank8 = rank7 << 8;

const bit_board upSide = rank1 | rank2 | rank3 | rank4;
const bit_board downSide = ~upSide;

const bit_board notRank12 = ~(rank1 | rank2);
const bit_board notRank78 = ~(rank7 | rank8);

inline bit_board shift(bit_board bitboard, int amount) {
    if (amount > 0) {
        return bitboard << amount;
    }
    return bitboard >> -amount;
}

inline int getIndex(bit_board bitboard) {
    return __builtin_ctzll(bitboard);
}

inline bool containsSquare(bit_board bitboard, integer square)
{
    return ((bitboard >> square) & 1) != 0;
}


#endif //CHESS_BITBOARDUTILITY_H
