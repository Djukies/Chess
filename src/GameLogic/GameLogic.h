#ifndef CHESS_GAMELOGIC_H
#define CHESS_GAMELOGIC_H

#include "../Types.h"
#include "../Board/Board.h"
#include <map>
#include <vector>
#include <cmath>
#include <chrono>


inline Move getSmallMove(integer oldPos, integer newPos, std::map<integer, std::vector<Move>> moves) {
    auto it = moves.find(oldPos); // Check if oldPos exists in the moves (Used to check if it is part of the right team for example)

    if (it != moves.end()) {
        auto found = std::find_if(it->second.begin(), it->second.end(), [newPos, oldPos](const Move &Move) {
            return (Move & startSquareMask) == oldPos && ((Move & targetSquareMask) >> 6) == newPos;
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

    int firstMadMove = board->fullMoves;


public:
    explicit GameLogic(Board* board);
    void undoMoves();
    bool tryMove(integer oldPos, integer newPos);
    std::map<integer, std::vector<Move>> moveVectorToMap(std::vector<Move> moves);
    std::vector<Move> madeMoves = {};
};


#endif //CHESS_GAMELOGIC_H
