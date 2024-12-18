#include "MoveGeneration.h"

/*
int8_t numOfSquaresToEdge(integer startSquare, int8_t dir) {
    Vector2 startSquareVector = intToVector2(startSquare);
    switch (dir) {
        case -9: // Diagonal up-left
            return std::min((int8_t) startSquareVector.x, (int8_t) startSquareVector.y);
        case -8: // Up
            return (int8_t) startSquareVector.y;
        case -7: // Diagonal up-right
            return std::min((int8_t) (7 - startSquareVector.x), (int8_t) startSquareVector.y);
        case -1: // Left
            return (int8_t) startSquareVector.x;
        case 1: // Right
            return (int8_t) (7 - startSquareVector.x);
        case 7: // Diagonal down-left
            return std::min((int8_t) startSquareVector.x, (int8_t) (7 - startSquareVector.y));
        case 8: // Down
            return (int8_t) (7 - startSquareVector.y);
        case 9: // Diagonal down-right
            return std::min((int8_t) (7 - startSquareVector.x), (int8_t) (7 - startSquareVector.y));
        default:
            throw std::invalid_argument("Invalid direction");
    }
}
*/


std::vector<Move> calculatePawnMoves(integer piece) {
    return {};
}

std::vector<Move> calculateKnightMoves(integer piece) {
    return {};
}

std::vector<Move> calculateKingMoves(integer piece) {
    return {};
}

std::vector<Move> calculateSlidingPieceMoves(integer piece, std::vector<int8_t> dirs) {
    return {};
}

std::map<integer, std::vector<Move>> calculateMoves(Board* board) {
    return {};
}