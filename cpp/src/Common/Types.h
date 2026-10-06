#ifndef CHESS_TYPES_H
#define CHESS_TYPES_H
#include <cstdint>
#include "raylib.h"
#include <iostream>
#include <vector>
#include <map>

#define bitboard uint_fast64_t
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

inline integer getStartsquare(Move move) {
    return move & startSquareMask;
}

inline integer getTargetsquare(Move move) {
    return (move & targetSquareMask) >> 6;
}

inline integer getFlag(Move move) {
    return (move & flagMask) >> 12;
}

const std::vector<int> pieceValues = {
    100,
    300,
    290,
    500,
    900,
    0,
    0
};

inline Move create_move(integer startSquare, integer targetSquare, Flag flag = NoFlag) {
    return (startSquare | targetSquare << 6 | flag << 12);
}

struct MadeMove {
    bool captured = false;
    Piece capturedPieceType = NONE;
    integer capturePosPlace = 0;
    integer prevEnPassant = 0;
    integer prevCastleRights = 0;

    bool operator==(const MadeMove & made_move) const = default;
};

struct Board {
    bitboard whitePieces = 0ULL;
    bitboard blackPieces = 0ULL;
    bitboard pawns = 0ULL;
    bitboard knights = 0ULL;
    bitboard bishops = 0ULL;
    bitboard rooks = 0ULL;
    bitboard queens = 0ULL;
    bitboard kings = 0ULL;
    bitboard allPieces = 0ULL;
    bitboard friendlyPieces = 0ULL;
    bitboard emptySquares = 0ULL;
    bitboard enemyPieces = 0ULL;
    integer blackKingPos = 0;
    integer whiteKingPos = 0;
    integer friendlyKingPos = 0;
    integer opponentKingPos = 0;
    bitboard pinnedPieces = 0ULL;
    bitboard checkRay = 0ULL;
    bool inDoubleCheck = false;
    bool inCheck = false;
    bitboard opponentControlledSquares = 0ULL;
    std::vector<Move> movesVector = {};
    std::map<integer, std::vector<Move>> movesMap = {};
    bitboard movesBitboard = 0ULL;
    bitboard pawnMoves = 0ULL;
    bitboard knightMoves = 0ULL;
    bitboard bishopMoves = 0ULL;
    bitboard rookMoves = 0ULL;
    bitboard queenMoves = 0ULL;
    bitboard kingMoves = 0ULL;
    uint8_t castleRights = 0;
    integer enPassantSquare = 0; // First bit says if enPassant is possible
    int halfMoves = 0;
    int fullMoves = 0;
    bool checkMate = false;
    bool checkMateWhite = false;
    bool staleMate = false;
    bool whiteToMove = TEAMWHITE;
    bool algorithmWhite = false;
    bool algorithmBlack = false;
    uint64_t zobristKey = 0;
    std::vector<uint64_t> history = {}; // hashes since last irreversible move
    std::string startingFen = "";
    std::vector<Move> movesMade = {};
    int firstMadMove = 0;
    std::vector<Move> madeMoves = {};
};

inline Vector2 intToVector2(integer pos) {
    integer x = pos%8;
    integer y = pos/8;
    return {(float)x, (float)y};
}

inline integer charToInt(std::string pos) {
    // Basic validation: must be at least 2 characters (e.g., "a1")
    if (pos.length() < 2) {
        std::cerr << "Error: Invalid position string length: " << pos << std::endl;
        return -1; // Indicate error
    }

    // 1. Calculate File Index (0-7, where 'a' is 0)
    int file = pos[0] - 'a';

    // 2. Calculate Rank Index (0-7, where '1' is 0)
    // Note: The board is usually indexed from the bottom (rank 1) up.
    int rank = '8' - pos[1];

    // Additional basic validation for valid chess coordinates
    if (file < 0 || file > 7 || rank < 0 || rank > 7) {
        std::cerr << "Error: Position coordinates out of bounds: " << pos << std::endl;
        return -1;
    }

    // 3. Calculate 0-63 index: (rank * 8) + file
    return (rank * 8) + file;
}

inline std::string intToString(integer pos) {
    // Basic validation: must be at least 2 characters (e.g., "a1")
    Vector2 posVector = intToVector2(pos);

    // 1. Calculate File Index (0-7, where 'a' is 0)
    int file = posVector.x;
    int rank = posVector.y;

    char fileChar = 'a' + file;
    std::string fileStr;
    fileStr = fileChar;
    char rankChar = '8' - rank;
    std::string rankStr;
    rankStr = rankChar;

    return fileStr + rankStr;
}

inline std::string moveToString(Move move) {
    if (move == 0) return "invalidmove";
    return intToString(getStartsquare(move)) + intToString(getTargetsquare(move));
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

struct PerftResult {
    std::map<std::string, int> move_nodes; // e.g., {"a2a3": 2186, "b2b3": 1964, ...}
    long long total_nodes = 0;             // The final "Nodes searched" total
    std::string full_breakdown;            // The raw move breakdown text

};

#endif //CHESS_TYPES_H
