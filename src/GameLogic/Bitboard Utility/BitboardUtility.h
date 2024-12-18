#include "../../Types.h"

#ifndef CHESS_BITBOARDUTILITY_H
#define CHESS_BITBOARDUTILITY_H

const bit_board fileA = 0x0101010101010101;
const bit_board fileH = fileA << 7;
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

inline bit_board shift(bit_board bitboard, int amount) {
    if (amount > 0) {
        return bitboard << amount;
    }
    return bitboard >> -amount;
}

inline int getIndex(bit_board bitboard) {
    return __builtin_ctzll(bitboard);
}


#endif //CHESS_BITBOARDUTILITY_H
