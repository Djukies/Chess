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

class GameLogic {
private:
    Board* board;

    std::vector<Move> calculatePawnMoves(integer piece);
    std::vector<Move> calculateKnightMoves(integer piece);
    std::vector<Move> calculateKingMoves(integer piece);
    std::vector<Move> calculateSlidingPieceMoves(integer piece, std::vector<int8_t> dirs);
    int firstMadMove = board->fullMoves;


public:
    explicit GameLogic(Board* board);
    void undoMoves();
    bool tryMove(integer oldPos, integer newPos);

    std::vector<Move> madeMoves = {};
    std::map<integer, std::vector<Move>> moves = calculateMoves();
    std::map<integer, std::vector<Move>> calculateMoves(bool recursion = false);
};


#endif //CHESS_GAMELOGIC_H
