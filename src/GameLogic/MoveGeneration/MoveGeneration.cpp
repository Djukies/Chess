#include "MoveGeneration.h"

void generatePromotions(integer startSquare, integer targetSquare, std::vector<small_move>& moves) {
    moves.push_back(create_move(startSquare, targetSquare, PromoteToBishopFlag));
    moves.push_back(create_move(startSquare, targetSquare, PromoteToKnightFlag));
    moves.push_back(create_move(startSquare, targetSquare, PromoteToQueenFlag));
    moves.push_back(create_move(startSquare, targetSquare, PromoteToRookFlag));
}

std::vector<small_move> calculatePawnMoves(Board* board) {
    std::vector<small_move> moves;
    bit_board pawns = board->pawns & board->friendlyPieces;
    int pushDir = board->whiteToMove ? 1 : -1;

    bit_board promotionRankMask = board->whiteToMove ? rank8 : rank1;
    bit_board singlePush = shift(pawns, pushDir*8) & board->emptySquares;

    bit_board doublePushTargetRank = board->whiteToMove ? rank4 : rank5;
    bit_board doublePush = shift(singlePush, pushDir*8) & doublePushTargetRank & board->emptySquares & board->checkRay;
    bit_board singlePushNoPromotions = singlePush & ~promotionRankMask & board->checkRay;
    bit_board pushPromotions = singlePush & promotionRankMask;

    bit_board captureEdgePreventionMask1 = board->whiteToMove ? notFileA : notFileH;
    bit_board captureEdgePreventionMask2 = board->whiteToMove ? notFileH : notFileA;
    bit_board captureA = shift(pawns & captureEdgePreventionMask1, pushDir * 7) & board->enemyPieces & board->checkRay;
    bit_board captureB = shift(pawns & captureEdgePreventionMask2, pushDir * 9) & board->enemyPieces & board->checkRay;

    bit_board capturePromotionsA = captureA & promotionRankMask;
    bit_board capturePromotionsB = captureB & promotionRankMask;

    captureA &= ~promotionRankMask;
    captureB &= ~promotionRankMask;

    while (singlePushNoPromotions != 0) {
        int targetSquare = getIndex(singlePushNoPromotions);
        singlePushNoPromotions &= singlePushNoPromotions-1;
        int startSquare = targetSquare - pushDir*8;
        moves.push_back(create_move(startSquare, targetSquare));
    }
    while (doublePush != 0) {
        int targetSquare = getIndex(doublePush);
        doublePush &= doublePush-1;
        int startSquare = targetSquare - pushDir*16;
        moves.push_back(create_move(startSquare, targetSquare, DoublePush));
    }
    while (captureA != 0) {
        int targetSquare = getIndex(captureA);
        captureA &= captureA-1;
        int startSquare = targetSquare - pushDir*7;
        moves.push_back(create_move(startSquare, targetSquare));
    }
    while (captureB != 0) {
        int targetSquare = getIndex(captureB);
        captureB &= captureB-1;
        int startSquare = targetSquare - pushDir*9;
        moves.push_back(create_move(startSquare, targetSquare));
    }
    while (pushPromotions != 0) {
        int targetSquare = getIndex(pushPromotions);
        pushPromotions &= pushPromotions-1;
        int startSquare = targetSquare - pushDir*8;
        generatePromotions(startSquare, targetSquare, moves);
    }
    while (capturePromotionsA != 0) {
        int targetSquare = getIndex(capturePromotionsA);
        capturePromotionsA &= capturePromotionsA-1;
        int startSquare = targetSquare - pushDir*8;
        generatePromotions(startSquare, targetSquare, moves);
    }
    while (capturePromotionsB != 0) {
        int targetSquare = getIndex(capturePromotionsB);
        capturePromotionsB &= capturePromotionsB-1;
        int startSquare = targetSquare - pushDir*8;
        generatePromotions(startSquare, targetSquare, moves);
    }

    return moves;
}

std::vector<small_move> calculateKnightMoves(Board* board) {
    std::vector<small_move> moves;
    bit_board knights = board->knights & board->friendlyPieces;
    while (knights != 0) {
        integer startSquare = getIndex(knights);
        knights &= knights-1;
        if (containsSquare(board->pinnedPieces, startSquare)) {
            continue;
        }
        bit_board possibleMoves = possibleKnightMoves[startSquare] & (board->emptySquares | board->enemyPieces) & board->checkRay;
        while (possibleMoves != 0) {
            integer targetSquare = getIndex(possibleMoves);
            possibleMoves &= possibleMoves - 1;
            moves.push_back(create_move(startSquare, targetSquare));
        }
    }

    return moves;
}

std::vector<small_move> calculateKingMoves(Board* board) {
    std::vector<small_move> moves;
    bit_board possibleMoves = possibleKingMoves[board->friendlyKingPos] & (board->emptySquares | board->enemyPieces) & (~board->opponentControlledSquares);
    while (possibleMoves != 0) {
        integer targetSquare = getIndex(possibleMoves);
        possibleMoves &= possibleMoves-1;
        moves.push_back(create_move(board->friendlyKingPos, targetSquare));
    }

    return moves;
}

std::vector<small_move> calculateSlidingMoves(Board* board, integer pos, int startDir, int endDir) {
    std::vector<small_move> moves;
    for (int dir = startDir; dir < endDir; dir++) {
        for (int distance = 1; distance <= distanceToEdge[pos][dir]; distance++) {
            if (containsSquare(board->allPieces, pos + directions[dir] * distance)) {
                if (containsSquare(board->enemyPieces, pos + directions[dir] * distance)) {
                    if (containsSquare(board->checkRay, pos + directions[dir]*distance)) {
                        moves.push_back(create_move(pos, pos + directions[dir] * distance));
                    }
                }
                break;
            }
            if (containsSquare(board->checkRay, pos + directions[dir]*distance)) {
                moves.push_back(create_move(pos, pos + directions[dir] * distance));
            }
        }
    }

    return moves;
}

std::vector<small_move> calculateSlidingPieceMoves(Board* board) {
    std::vector<small_move> moves;
    bit_board diagonalSliders = board->friendlyPieces & (board->bishops | board->queens);
    bit_board othogonalSliders = board->friendlyPieces & (board->rooks | board->queens);

    while (diagonalSliders != 0) {
        int startSquare = getIndex(diagonalSliders);
        diagonalSliders &= diagonalSliders-1;
        std::vector<small_move> movesOfPiece = calculateSlidingMoves(board, startSquare, 4, 8);
        moves.insert(moves.end(), movesOfPiece.begin(), movesOfPiece.end());
    }
    while (othogonalSliders != 0) {
        int startSquare = getIndex(othogonalSliders);
        othogonalSliders &= othogonalSliders-1;
        std::vector<small_move> movesOfPiece = calculateSlidingMoves(board, startSquare, 0, 4);
        moves.insert(moves.end(), movesOfPiece.begin(), movesOfPiece.end());
    }

    return moves;
}

bit_board calculateSlidingMovesBitboard(Board* board, integer pos, int startDir, int endDir) {
    bit_board moves = 0ULL;
    for (int dir = startDir; dir < endDir; dir++) {
        for (int distance = 1; distance <= distanceToEdge[pos][dir]; distance++) {
            if (containsSquare(board->allPieces, pos + directions[dir] * distance)) {
                moves |= 1ULL << (pos + directions[dir] * distance);
                break;
            }
            moves |= 1ULL << (pos + directions[dir] * distance);
        }
    }

    return moves;
}

bit_board calculateSlidingPieceMovesAttackMap(Board* board) {
    bit_board moves = 0ULL;
    bit_board diagonalSliders = board->enemyPieces & (board->bishops | board->queens);
    bit_board othogonalSliders = board->enemyPieces & (board->rooks | board->queens);

    while (diagonalSliders != 0) {
        int startSquare = getIndex(diagonalSliders);
        diagonalSliders &= diagonalSliders-1;
        bit_board movesOfPiece = calculateSlidingMovesBitboard(board, startSquare, 4, 8);
        moves |= movesOfPiece;
    }
    while (othogonalSliders != 0) {
        int startSquare = getIndex(othogonalSliders);
        othogonalSliders &= othogonalSliders-1;
        bit_board movesOfPiece = calculateSlidingMovesBitboard(board, startSquare, 0, 4);
        moves |= movesOfPiece;
    }

    return moves;
}

bit_board calculatePawnAttacks(bit_board pawns, bool isWhiteToMove) {
    // Calculated by first moving the pawns to the place they can attack (pawns << 9)
    // And then removing the A-File, because if they moved right, they can't be on the A-file

    if (isWhiteToMove) {
        return ((pawns << 9) & notFileA) | ((pawns << 7) & notFileH);
    }
    return ((pawns >> 7) & notFileA) | ((pawns >> 9) & notFileH);
}

void calculateAttackData(Board* board) {
    int startIndex = 0;
    int endIndex = 8;
    if (board->queens == 0) {
        startIndex = board->rooks == 0 ? 4 : 8;
        endIndex = board->bishops == 0 ? 8 : 4;
    }
    for (int dir = startIndex; dir < endIndex; dir++) {
        bool isDiagonal = dir >= 4;
        bit_board slidingPieces = board->queens;
        slidingPieces |= isDiagonal ? board->bishops : board->rooks;
        slidingPieces &= board->enemyPieces;
        if ((rays[board->friendlyKingPos][dir] & slidingPieces) == 0) {
            continue;
        }
        integer friendlyPieceAlongRay = 0;
        int directionOffset = directions[dir];
        bit_board rayMask = 0ULL;
        for (int n = 1; n <= distanceToEdge[board->friendlyKingPos][dir]; n++) {
            int pos = board->friendlyKingPos + n*directionOffset;
            rayMask |= 1ULL << pos;
            if (containsSquare(board->allPieces, pos)) {
                if (containsSquare(board->friendlyPieces, pos)) {
                    if (friendlyPieceAlongRay != 0) { // There already is a friendlyPiece along the ray, so it can't be pinned
                        break;
                    } else {
                        friendlyPieceAlongRay = pos;
                    }
                } else {
                    if (containsSquare(slidingPieces, pos)) {
                        if (friendlyPieceAlongRay != 0) {
                            board->pinnedPieces |= 1ULL << friendlyPieceAlongRay;
                        } else {
                            board->checkRay |= rayMask;
                            board->inDoubleCheck = board->inCheck; // If already in check, it is in double check
                            board->inCheck = true;
                        }
                        break;
                    } else {
                        break;
                    }
                }
            }
        }
        if (board->inDoubleCheck) {
            break;
            // If in double check, only the king can move, so the pinned pieces don't matter
        }
    }
    bit_board opponentSlidingPieceAttacks = calculateSlidingPieceMovesAttackMap(board);

    bit_board friendlyKingMask = 1ULL << board->friendlyKingPos;
    bit_board knights = board->knights & board->enemyPieces;
    bit_board opponentKnightAttacks = 0ULL;

    while (knights != 0) {
        integer startSquare = getIndex(knights);
        knights &= knights-1;
        bit_board knightAttacks = possibleKnightMoves[startSquare];
        opponentKnightAttacks |= knightAttacks;
        if ((knightAttacks & friendlyKingMask) != 0) {
            board->checkRay |= 1ULL << startSquare;
            board->inDoubleCheck = board->inCheck;
            board->inCheck = true;
        }
    }

    bit_board pawnAttacks = calculatePawnAttacks(board->pawns & board->enemyPieces, !board->whiteToMove);

    if (containsSquare(pawnAttacks, board->friendlyKingPos)) {
        board->inDoubleCheck = board->inCheck;
        board->inCheck = true;
        bit_board possibleAttackOrigins = board->whiteToMove ? (board->friendlyKingPos >> 7) | (board->friendlyKingPos >> 9) : (board->friendlyKingPos << 7) | (board->friendlyKingPos << 9);
        board->checkRay |= (board->pawns & board->enemyPieces) & possibleAttackOrigins;
    }

    bit_board opponentKingAttacks = possibleKingMoves[board->opponentKingPos];

    board->opponentControlledSquares = opponentKnightAttacks | opponentSlidingPieceAttacks | opponentKingAttacks | pawnAttacks;
    if (!board->inCheck) {
        board->checkRay = UINT64_MAX; // Can only move on places to block check, so if no check pieces can move everywhere
    } else {
        board->checkRay |= 0ULL;
    }
}

void init(Board* board) {
    if (board->whiteToMove) {
        board->friendlyPieces = board->whitePieces;
        board->enemyPieces = board->blackPieces;
        board->friendlyKingPos = board->whiteKingPos;
        board->opponentKingPos = board->blackKingPos;
    } else {
        board->friendlyPieces = board->blackPieces;
        board->enemyPieces = board->whitePieces;
        board->friendlyKingPos = board->blackKingPos;
        board->opponentKingPos = board->whiteKingPos;
    }
    board->checkRay = 0ULL;
    board->pinnedPieces = 0ULL;
    board->inCheck = false;
    board->inDoubleCheck = false;
    board->opponentControlledSquares = 0ULL;
    board->emptySquares = ~(board->whitePieces | board->blackPieces);
    board->allPieces = board->blackPieces | board->whitePieces;

    calculateAttackData(board);
}

std::vector<small_move> calculateLegalMoves(Board* board) {
    std::vector<small_move> moves;
    init(board);

    std::vector<small_move> kingMoves = calculateKingMoves(board);
    moves.insert(moves.end(), kingMoves.begin(), kingMoves.end());
    if (!board->inDoubleCheck) {
        std::vector<small_move> pawnMoves = calculatePawnMoves(board);
        std::vector<small_move> knightMoves = calculateKnightMoves(board);
        std::vector<small_move> sliderMoves = calculateSlidingPieceMoves(board);

        moves.insert(moves.end(), pawnMoves.begin(), pawnMoves.end());
        moves.insert(moves.end(), knightMoves.begin(), knightMoves.end());
        moves.insert(moves.end(), sliderMoves.begin(), sliderMoves.end());
    }
    /*std::cout << "ALLMOVES" << std::endl;
    for (small_move move : moves) {
        std::cout << "Start: " << (int)(move & startSquareMask) << ", End: " << (int)((move & targetSquareMask) >> 6) << ", Flags: " << (int)((move & flagMask) >> 12) << std::endl;
    }
    std::cout << "PAWNS:" << std::endl;
    for (small_move move : calculatePawnMoves(board)) {
        std::cout << "Start: " << (int)(move & startSquareMask) << ", End: " << (int)((move & targetSquareMask) >> 6) << ", Flags: " << (int)((move & flagMask) >> 12) << std::endl;
    }
    std::cout << "KINGS:" << std::endl;
    for (small_move move : kingMoves) {
        std::cout << "Start: " << (int)(move & startSquareMask) << ", End: " << (int)((move & targetSquareMask) >> 6) << ", Flags: " << (int)((move & flagMask) >> 12) << std::endl;
    }
    std::cout << "KNIGHTS:" << std::endl;
    for (small_move move : knightMoves) {
        std::cout << "Start: " << (int)(move & startSquareMask) << ", End: " << (int)((move & targetSquareMask) >> 6) << ", Flags: " << (int)((move & flagMask) >> 12) << std::endl;
    }
    std::cout << "SLIDERS" << std::endl;
    for (small_move move : sliderMoves) {
        std::cout << "Start: " << (int)(move & startSquareMask) << ", End: " << (int)((move & targetSquareMask) >> 6) << ", Flags: " << (int)((move & flagMask) >> 12) << std::endl;
    }
    std::cout << "Amount Of Moves: " << moves.size() << std::endl;*/
    return moves;
}