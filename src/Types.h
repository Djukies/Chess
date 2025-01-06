#ifndef CHESS_TYPES_H
#define CHESS_TYPES_H
#include <cstdint>
#include "raylib.h"
#include <iostream>
#include <vector>
#include <map>

#define bit_board uint_fast64_t
#define Move uint16_t
#define integer uint8_t
#define TEAMWHITE true
#define TEAMBLACK false


enum Piece {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    NONE
};

enum Flag {
    NoFlag,
    EnPassantCaptureFlag,
    CastleFlag,
    DoublePush,

    PromoteToQueenFlag,
    PromoteToKnightFlag,
    PromoteToRookFlag,
    PromoteToBishopFlag,
};

const Move startSquareMask = 0b0000000000111111;
const Move targetSquareMask = 0b0000111111000000;
const Move flagMask = 0b1111000000000000;

inline Move create_move(integer startSquare, integer targetSquare, Flag flag = NoFlag) {
    return (startSquare | targetSquare << 6 | flag << 12);
}

struct MadeMove {
    bool captured = false;
    Piece capturedPieceType = NONE;
    integer capturePosPlace = 0;
    integer prevEnPassant = 0;
};

struct Board {
    bit_board whitePieces = 0ULL;
    bit_board blackPieces = 0ULL;
    bit_board pawns = 0ULL;
    bit_board knights = 0ULL;
    bit_board bishops = 0ULL;
    bit_board rooks = 0ULL;
    bit_board queens = 0ULL;
    bit_board kings = 0ULL;
    bit_board allPieces = 0ULL;
    bit_board friendlyPieces = 0ULL;
    bit_board emptySquares = 0ULL;
    bit_board enemyPieces = 0ULL;
    integer blackKingPos = 0;
    integer whiteKingPos = 0;
    integer friendlyKingPos = 0;
    integer opponentKingPos = 0;
    bit_board pinnedPieces = 0ULL;
    bit_board checkRay = 0ULL;
    bool inDoubleCheck = false;
    bool inCheck = false;
    bit_board opponentControlledSquares = 0ULL;
    std::vector<Move> movesVector = {};
    std::map<integer, std::vector<Move>> movesMap = {};
    uint8_t castleRights = 0;
    integer enPassantSquare = 0; // First bit says if enPassant is possible
    int halfMoves = 0;
    int fullMoves = 0;
    bool whiteToMove = TEAMWHITE;
};

inline Vector2 intToVector2(integer pos) {
    integer x = pos%8;
    integer y = pos/8;
    return {(float)x, (float)y};
}

inline int intToX(int pos) {
    return pos%8;
}

inline int intToY(int pos) {
    return pos/8;
}

inline integer vector2ToInt(Vector2 pos) {
    return (integer)pos.x + (integer)pos.y*8;
}

inline integer vector2ScreenToInt(Vector2 screenPos, int squareSize) {
    integer x = (int)screenPos.x/squareSize;
    integer y = (int)screenPos.y/squareSize;
    return (integer)(x+y*8);
}

inline Vector2 intToVector2ScreenPos(integer pos, int squareSize) {
    int x = (pos%8) * squareSize;
    int y = (pos/8) * squareSize;
    return {(float)x, (float)y};
}

#endif //CHESS_TYPES_H
