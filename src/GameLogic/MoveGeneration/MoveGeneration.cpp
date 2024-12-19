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
    bit_board doublePush = shift(singlePush, pushDir*8) & doublePushTargetRank;
    bit_board singlePushNoPromotions = singlePush & ~promotionRankMask;
    bit_board pushPromotions = singlePush & promotionRankMask;

    bit_board captureEdgePreventionMask1 = board->whiteToMove ? notFileA : notFileH;
    bit_board captureEdgePreventionMask2 = board->whiteToMove ? notFileH : notFileA;
    bit_board captureA = shift(pawns & captureEdgePreventionMask1, pushDir * 7) & board->enemyPieces;
    bit_board captureB = shift(pawns & captureEdgePreventionMask2, pushDir * 9) & board->enemyPieces;

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
        bit_board possibleMoves = knightMoves[startSquare] & (board->emptySquares | board->enemyPieces);
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
    bit_board possibleMoves = kingMoves[board->friendlyKingPos] & (board->emptySquares | board->enemyPieces);
    while (possibleMoves != 0) {
        integer targetSquare = getIndex(possibleMoves);
        possibleMoves &= possibleMoves-1;
        moves.push_back(create_move(board->friendlyKingPos, targetSquare));
    }

    return moves;
}

std::vector<Move> calculateSlidingPieceMoves(bit_board piece, integer startdir, integer enddir) {
    return {};
}

std::vector<Move> calculateLegalMoves(Board* board) {
    std::cout << "bals" << std::endl;
    board->friendlyPieces = board->whitePieces;
    board->emptySquares = ~(board->whitePieces | board->blackPieces);
    board->enemyPieces = board->blackPieces;
    board->friendlyKingPos = board->whiteKingPos;
    std::cout << "PAWNS:" << std::endl;
    for (small_move move : calculatePawnMoves(board)) {
        std::cout << "Start: " << (int)(move & startSquareMask) << ", End: " << (int)((move & targetSquareMask) >> 6) << ", Flags: " << (int)((move & flagMask) >> 12) << std::endl;
    }
    std::cout << "KINGS:" << std::endl;
    for (small_move move : calculateKingMoves(board)) {
        std::cout << "Start: " << (int)(move & startSquareMask) << ", End: " << (int)((move & targetSquareMask) >> 6) << ", Flags: " << (int)((move & flagMask) >> 12) << std::endl;
    }
    std::cout << "KNIGHTS:" << std::endl;
    for (small_move move : calculateKnightMoves(board)) {
        std::cout << "Start: " << (int)(move & startSquareMask) << ", End: " << (int)((move & targetSquareMask) >> 6) << ", Flags: " << (int)((move & flagMask) >> 12) << std::endl;
    }
    return {};
}