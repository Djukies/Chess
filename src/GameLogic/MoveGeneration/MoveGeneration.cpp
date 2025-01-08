#include "MoveGeneration.h"

bool isPinned(Board* board, integer piece) {
    return containsSquare(board->pinnedPieces, piece);
}

void generatePromotions(integer startSquare, integer targetSquare, std::vector<Move>& moves) {
    moves.push_back(create_move(startSquare, targetSquare, PromoteToBishopFlag));
    moves.push_back(create_move(startSquare, targetSquare, PromoteToKnightFlag));
    moves.push_back(create_move(startSquare, targetSquare, PromoteToQueenFlag));
    moves.push_back(create_move(startSquare, targetSquare, PromoteToRookFlag));
}

std::vector<Move> calculatePawnMoves(Board* board) {
    std::vector<Move> moves;
    bit_board pawns = board->pawns & board->friendlyPieces;
    int pushDir = board->whiteToMove ? -1 : 1;

    bit_board promotionRankMask = board->whiteToMove ? rank1 : rank8;
    bit_board singlePush = shift(pawns, pushDir*8) & board->emptySquares;

    bit_board doublePushTargetRank = board->whiteToMove ? rank5 : rank4;
    bit_board doublePush = shift(singlePush, pushDir*8) & doublePushTargetRank & board->emptySquares & board->checkRay;
    bit_board singlePushNoPromotions = singlePush & ~promotionRankMask & board->checkRay;
    bit_board pushPromotions = singlePush & promotionRankMask;

    bit_board captureEdgePreventionMask1 = board->whiteToMove ? notFileH : notFileA;
    bit_board captureEdgePreventionMask2 = board->whiteToMove ? notFileA : notFileH;

    bit_board enPassantSquare = 1ULL << (board->enPassantSquare + 8*pushDir);

    bit_board captureA = shift(pawns & captureEdgePreventionMask1, pushDir * 7) & (board->enemyPieces | enPassantSquare) & board->checkRay;
    bit_board captureB = shift(pawns & captureEdgePreventionMask2, pushDir * 9) & (board->enemyPieces | enPassantSquare) & board->checkRay;

    bit_board capturePromotionsA = captureA & promotionRankMask;
    bit_board capturePromotionsB = captureB & promotionRankMask;

    captureA &= ~promotionRankMask;
    captureB &= ~promotionRankMask;


    while (singlePushNoPromotions != 0) {
        int targetSquare = getIndex(singlePushNoPromotions);
        singlePushNoPromotions &= singlePushNoPromotions-1;
        int startSquare = targetSquare - pushDir*8;
        if (!isPinned(board, startSquare) || getRays(board->friendlyKingPos, startSquare) == getRays(board->friendlyKingPos, targetSquare)) {
            moves.push_back(create_move(startSquare, targetSquare));
        }
    }
    while (doublePush != 0) {
        int targetSquare = getIndex(doublePush);
        doublePush &= doublePush-1;
        int startSquare = targetSquare - pushDir*16;
        if (!isPinned(board, startSquare) || getRays(board->friendlyKingPos, startSquare) == getRays(board->friendlyKingPos, targetSquare)) {
            moves.push_back(create_move(startSquare, targetSquare, DoublePush));
        }
    }
    while (captureA != 0) {
        int targetSquare = getIndex(captureA);
        captureA &= captureA-1;
        int startSquare = targetSquare - pushDir*7;
        if (!isPinned(board, startSquare) || getRays(board->friendlyKingPos, startSquare) == getRays(board->friendlyKingPos, targetSquare)) {
            if (targetSquare == board->enPassantSquare + 8*pushDir) {
                moves.push_back(create_move(startSquare, targetSquare, EnPassantCaptureFlag));
            } else {
                moves.push_back(create_move(startSquare, targetSquare));
            }
        }
    }
    while (captureB != 0) {
        int targetSquare = getIndex(captureB);
        captureB &= captureB-1;
        int startSquare = targetSquare - pushDir*9;
        if (!isPinned(board, startSquare) || getRays(board->friendlyKingPos, startSquare) == getRays(board->friendlyKingPos, targetSquare)) {
            if (targetSquare == board->enPassantSquare + 8*pushDir) {
                moves.push_back(create_move(startSquare, targetSquare, EnPassantCaptureFlag));
            } else {
                moves.push_back(create_move(startSquare, targetSquare));
            }
        }
    }
    while (pushPromotions != 0) {
        int targetSquare = getIndex(pushPromotions);
        pushPromotions &= pushPromotions-1;
        int startSquare = targetSquare - pushDir*8;
        if (!isPinned(board, startSquare)) {
            generatePromotions(startSquare, targetSquare, moves);
        }
    }
    while (capturePromotionsA != 0) {
        int targetSquare = getIndex(capturePromotionsA);
        capturePromotionsA &= capturePromotionsA-1;
        int startSquare = targetSquare - pushDir*8;
        if (!isPinned(board, startSquare) || getRays(board->friendlyKingPos, startSquare) == getRays(board->friendlyKingPos, targetSquare)) {
            generatePromotions(startSquare, targetSquare, moves);
        }
    }
    while (capturePromotionsB != 0) {
        int targetSquare = getIndex(capturePromotionsB);
        capturePromotionsB &= capturePromotionsB-1;
        int startSquare = targetSquare - pushDir*8;
        if (!isPinned(board, startSquare) || getRays(board->friendlyKingPos, startSquare) == getRays(board->friendlyKingPos, targetSquare)) {
            generatePromotions(startSquare, targetSquare, moves);
        }
    }

    return moves;
}

std::vector<Move> calculateKnightMoves(Board* board) {
    std::vector<Move> moves;
    bit_board knights = board->knights & board->friendlyPieces;
    while (knights != 0) {
        integer startSquare = getIndex(knights);
        knights &= knights-1;
        if (!isPinned(board, startSquare)) { // If knight is pinned, it will never be able to Move
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
    }

    return moves;
}

std::vector<Move> calculateKingMoves(Board* board) {
    std::vector<Move> moves;
    bit_board possibleMoves = possibleKingMoves[board->friendlyKingPos] & (board->emptySquares | board->enemyPieces) & (~board->opponentControlledSquares);
    while (possibleMoves != 0) {
        integer targetSquare = getIndex(possibleMoves);
        possibleMoves &= possibleMoves-1;
        moves.push_back(create_move(board->friendlyKingPos, targetSquare));
    }

    return moves;
}

std::vector<Move> calculateSlidingMoves(Board* board, integer pos, int startDir, int endDir) {
    std::vector<Move> moves;
    for (int dir = startDir; dir < endDir; dir++) {
        for (int distance = 1; distance <= distanceToEdge[pos][dir]; distance++) {
            if (containsSquare(board->allPieces, pos + directions[dir] * distance)) {
                if (containsSquare(board->enemyPieces, pos + directions[dir] * distance)) {
                    if (containsSquare(board->checkRay, pos + directions[dir]*distance) && (!isPinned(board, pos) || getRays(board->friendlyKingPos, pos) == getRays(board->friendlyKingPos, pos + directions[dir] * distance))) {
                        moves.push_back(create_move(pos, pos + directions[dir] * distance));
                    }
                }
                break;
            }
            if (containsSquare(board->checkRay, pos + directions[dir]*distance) && (!isPinned(board, pos) || getRays(board->friendlyKingPos, pos) == getRays(board->friendlyKingPos, pos + directions[dir] * distance))) {
                moves.push_back(create_move(pos, pos + directions[dir] * distance));
            }
        }
    }

    return moves;
}

std::vector<Move> calculateSlidingPieceMoves(Board* board) {
    std::vector<Move> moves;
    bit_board diagonalSliders = board->friendlyPieces & (board->bishops | board->queens);
    bit_board othogonalSliders = board->friendlyPieces & (board->rooks | board->queens);

    while (diagonalSliders != 0) {
        int startSquare = getIndex(diagonalSliders);
        diagonalSliders &= diagonalSliders-1;
        std::vector<Move> movesOfPiece = calculateSlidingMoves(board, startSquare, 4, 8);
        moves.insert(moves.end(), movesOfPiece.begin(), movesOfPiece.end());
    }
    while (othogonalSliders != 0) {
        int startSquare = getIndex(othogonalSliders);
        othogonalSliders &= othogonalSliders-1;
        std::vector<Move> movesOfPiece = calculateSlidingMoves(board, startSquare, 0, 4);
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
        return ((pawns >> 7) & notFileA) | ((pawns >> 9) & notFileH);
    }
    return ((pawns << 9) & notFileA) | ((pawns << 7) & notFileH);
}

void calculateAttackData(Board* board) {
    int startIndex = 0;
    int endIndex = 8;
    if ((board->queens & board->enemyPieces) == 0) {
        startIndex = (board->rooks & board->enemyPieces) == 0 ? 4 : 0;
        endIndex = (board->bishops & board->enemyPieces) == 0 ? 8 : 4;
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
            // If in double check, only the king can Move, so the pinned pieces don't matter
        }
    }

    // Calculate all the attack Possibilities
    // First remove the king, so it can't hide behind himself
    board->allPieces &= ~(1ULL << board->friendlyKingPos);

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
    // Add the king back at the end
    board->allPieces |= 1ULL << board->friendlyKingPos;

    if (!board->inCheck) {
        board->checkRay = UINT64_MAX; // Can only Move on places to block check, so if no check pieces can Move everywhere
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

std::vector<Move> calculateLegalMoves(Board* board) {
    std::vector<Move> moves;
    init(board);

    std::vector<Move> kingMoves = calculateKingMoves(board);
    moves.insert(moves.end(), kingMoves.begin(), kingMoves.end());
    if (!board->inDoubleCheck) {
        std::vector<Move> pawnMoves = calculatePawnMoves(board);
        std::vector<Move> knightMoves = calculateKnightMoves(board);
        std::vector<Move> sliderMoves = calculateSlidingPieceMoves(board);

        moves.insert(moves.end(), pawnMoves.begin(), pawnMoves.end());
        moves.insert(moves.end(), knightMoves.begin(), knightMoves.end());
        moves.insert(moves.end(), sliderMoves.begin(), sliderMoves.end());
    }
    return moves;
}