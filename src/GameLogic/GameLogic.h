#ifndef CHESS_GAMELOGIC_H
#define CHESS_GAMELOGIC_H

#include "../Types.h"
#include "../Board/Board.h"
#include <map>
#include <vector>
#include <cmath>
#include <chrono>

inline Move getMove(integer oldPos, integer newPos, std::map<integer, std::vector<Move>> allMoves) {
    auto it = allMoves.find(oldPos); // Check if oldPos exists in the moves (Used to check if it is part of the right team for example)

    if (it != allMoves.end()) {
        // Use std::find_if to match only the position
        auto found = std::find_if(it->second.begin(), it->second.end(),
                                  [newPos](const Move& move) { return move.newPos == newPos; });
        if (found != it->second.end()) {
            return {found->prevEnPassantSquare, found->move_type, found->oldPos, found->newPos, found->ownPieceType, found->team, found->specialPiece, found->specialPieceType, found->promotionPiece}; // Return the MoveType of the found move
        }
    }
    return {NO_MOVE};
}

inline small_move getSmallMove(integer oldPos, integer newPos, std::map<integer, std::vector<small_move>> moves) {
    auto it = moves.find(oldPos); // Check if oldPos exists in the moves (Used to check if it is part of the right team for example)

    if (it != moves.end()) {
        auto found = std::find_if(it->second.begin(), it->second.end(), [newPos, oldPos](const small_move &move) {
            return (move & startSquareMask) == oldPos && ((move & targetSquareMask) >> 6) == newPos;
        });
        if (found != it->second.end()) {
            return *found;
        }
    }
    return 0;
}

class GameLogic {
private:
    Board* board;

    std::vector<Move> calculatePawnMoves(integer piece);
    std::vector<Move> calculateKnightMoves(integer piece);
    std::vector<Move> calculateKingMoves(integer piece, bool onlyLegalMoves);
    std::vector<Move> calculateSlidingPieceMoves(integer piece, std::vector<int8_t> dirs);
    void setOpponentControlledSquares();
    int firstMadMove = board->fullMoves;


public:
    explicit GameLogic(Board* board);
    void undoMoves();
    bool tryMove(integer oldPos, integer newPos);
    std::map<integer, std::vector<small_move>> moveVectorToMap(std::vector<small_move> moves);
    std::vector<small_move> madeMoves = {};
    std::map<integer, std::vector<Move>> moves;
    std::map<integer, std::vector<Move>> calculateMoves(bool onlyLegalMoves = true);
};


#endif //CHESS_GAMELOGIC_H
