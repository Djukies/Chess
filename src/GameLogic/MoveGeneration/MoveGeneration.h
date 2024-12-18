#ifndef CHESS_MOVEGENERATION_H
#define CHESS_MOVEGENERATION_H

#include "../../Types.h"
#include <vector>
#include <map>

inline std::map<integer, std::vector<Move>> calculateLegalMoves(Board* board);

#endif //CHESS_MOVEGENERATION_H
