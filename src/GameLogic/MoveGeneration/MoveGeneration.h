#ifndef CHESS_MOVEGENERATION_H
#define CHESS_MOVEGENERATION_H

#include "../../Types.h"
#include "../PrecomputedData/PrecomputedData.h"
#include "../BitboardUtility/BitboardUtility.h"
#include <vector>
#include <map>

std::vector<Move> calculateLegalMoves(Board* board);

#endif //CHESS_MOVEGENERATION_H
