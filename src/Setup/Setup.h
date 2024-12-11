#ifndef CHESS_SETUP_H
#define CHESS_SETUP_H
#include <string>
#include <map>
#include <vector>
#include "../Types.h"

enum FENComponents {
    BOARD,
    ACTIVE_TEAM,
    CASTLE_RIGHTS,
    EN_PASSANT_PLACE,
    HALF_MOVES,
    FULL_MOVES
};

void loadAllBoard(Board* board, std::string fen);


#endif //CHESS_SETUP_H
