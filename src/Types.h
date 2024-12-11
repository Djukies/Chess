#ifndef CHESS_TYPES_H
#define CHESS_TYPES_H
#include <cstdint>
#include "raylib.h"
#include <iostream>
#define bit_board uint_fast64_t
#define integer uint8_t
#define TEAMWHITE true
#define TEAMBLACK false

enum Move_Type {
    NO_MOVE,
    NORMAL_MOVE,
    DOUBLE_PAWN_PUSH,
    CAPTURE,
    PROMOTION,
    CAPTURE_PROMOTION,
    EN_PASSANT,
    CASTLE
};

enum Piece {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    NONE
};


struct Move {
    integer prevEnPassantSquare = 0;
    Move_Type move_type = NO_MOVE;
    integer oldPos = 0;
    integer newPos = 0;
    Piece ownPieceType = NONE;
    bool team = TEAMWHITE;
    integer specialPiece = 0; // Captured piece or the rook while castling etc.
    Piece specialPieceType = NONE;
    Piece promotionPiece = QUEEN;

    bool operator==(const Move& other) const {
        return oldPos == other.oldPos &&
               ownPieceType == other.ownPieceType &&
               newPos == other.newPos &&
               move_type == other.move_type &&
               specialPiece == other.specialPiece &&
               specialPieceType == other.specialPieceType &&
               team == other.team &&
               promotionPiece == other.promotionPiece;
    }
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
    integer blackKingPos = 0;
    integer whiteKingPos = 0;
    uint8_t castleRights = 0;
    integer enPassantSquare = 0; // First bit says if enPassant is possible
    int halfMoves = 0;
    int fullMoves = 0;
    bool activeTeam = TEAMWHITE;
    bit_board opponentSquares = 0ULL;
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
