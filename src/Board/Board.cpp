#include "Board.h"


Board* loadBoardFromFEN(const std::string fen) {
    auto *board = new Board;
    loadAllBoard(board, fen);

    return board;
}

void replacePosToBitboards(Board* board, integer oldPos, integer newPos) {
    bit_board oldPosMask = 1ULL << oldPos;
    bit_board newPosMask = 1ULL << newPos;

    bit_board mask = oldPosMask | newPosMask;


    if (oldPosMask & board->pawns) {
        board->pawns ^= mask;
    } else if (oldPosMask & board->knights) {
        board->knights ^= mask;
    } else if (oldPosMask & board->bishops) {
        board->bishops ^= mask;
    } else if (oldPosMask & board->rooks) {
        board->rooks ^= mask;
    } else if (oldPosMask & board->queens) {
        board->queens ^= mask;
    } else if (oldPosMask & board->kings) {
        board->kings ^= mask;
        if (oldPosMask & board->whitePieces) {
            board->whiteKingPos = newPos;
        } else if (oldPosMask & board->blackPieces) {
            board->blackKingPos = newPos;
        }
    }

    if (oldPosMask & board->whitePieces) {
        board->whitePieces ^= mask;
    } else if (oldPosMask & board->blackPieces) {
        board->blackPieces ^= mask;
    }
}

void removeFromBitboards(Board* board, integer pos) {
    bit_board mask = 1ULL << pos;
    board->blackPieces &= ~mask;
    board->whitePieces &= ~mask;

    board->pawns &= ~mask;
    board->knights &= ~mask;
    board->bishops &= ~mask;
    board->rooks &= ~mask;
    board->queens &= ~mask;
    board->kings &= ~mask;
}

void addToBitboards(Board* board, integer pos, Piece pieceType, bool team) {
    bit_board mask = 1ULL << pos;
    if (team == TEAMWHITE) board->whitePieces |= mask;
    else board->blackPieces |= mask;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
    switch (pieceType) {
        case PAWN:
            board->pawns |= mask;
            break;
        case KNIGHT:
            board->knights |= mask;
            break;
        case BISHOP:
            board->bishops |= mask;
            break;
        case ROOK:
            board->rooks |= mask;
            break;
        case QUEEN:
            board->queens |= mask;
            break;
        case KING:
            board->kings |= mask;
            break;
    }
#pragma clang diagnostic pop
}


void makeMove(Board* board, Move move) {
    board->enPassantSquare = 0;
    if (move.move_type == NORMAL_MOVE) {
        replacePosToBitboards(board, move.oldPos, move.newPos);
    }
    else if (move.move_type == CAPTURE) {
        removeFromBitboards(board, move.newPos);
        replacePosToBitboards(board, move.oldPos, move.newPos);
    }
    else if (move.move_type == DOUBLE_PAWN_PUSH) {
        replacePosToBitboards(board, move.oldPos, move.newPos);
        board->enPassantSquare = move.newPos;
        board->enPassantSquare |= 1ULL << 7;
    }
    else if (move.move_type == EN_PASSANT) {
        removeFromBitboards(board, move.specialPiece);
        replacePosToBitboards(board, move.oldPos, move.newPos);
    }
    else if (move.move_type == PROMOTION) {
        removeFromBitboards(board, move.oldPos);
        addToBitboards(board, move.newPos, move.promotionPiece, move.team);
    }
    else if (move.move_type == CAPTURE_PROMOTION) {
        removeFromBitboards(board, move.oldPos);
        removeFromBitboards(board, move.specialPiece);
        addToBitboards(board, move.newPos, move.promotionPiece, move.team);
    }
    else if (move.move_type == CASTLE) {
    }

    board->whiteToMove = !board->whiteToMove;
    board->fullMoves ++;
}

std::vector<MadeMove> movesMade = {};

void makeSmallMove(Board* board, small_move move) {
    board->enPassantSquare = 0;
    integer startSquare = move & startSquareMask;
    integer targetSquare = (move & targetSquareMask) >> 6;
    int flag = ((move & flagMask) >> 12);
    MadeMove madeMove;
    switch (flag) {
        case NoFlag:
            if (containsSquare(board->blackPieces | board->whitePieces, targetSquare)) {
                madeMove.captured = true;
                madeMove.capturedPieceType = getPiece(board, targetSquare);
                madeMove.capturePosPlace = targetSquare;
                removeFromBitboards(board, targetSquare);
            }
            replacePosToBitboards(board, startSquare, targetSquare);
            break;
        case EnPassantCaptureFlag:
            break;
        case CastleFlag:
            break;
        case DoublePush:
            replacePosToBitboards(board, startSquare, targetSquare);
            board->enPassantSquare = targetSquare;
            break;
        default:
            break;
    }
    board->whiteToMove = !board->whiteToMove;
    board->fullMoves++;
    movesMade.push_back(madeMove);
}

void unMakeSmallMove(Board* board, small_move move) {
    integer targetSquare = move & startSquareMask;
    integer startSquare = (move & targetSquareMask) >> 6;
    int flag = ((move & flagMask) >> 12);
    MadeMove moveToUnmake = movesMade[movesMade.size()-1];

    switch (flag) {
        case NoFlag:
            replacePosToBitboards(board, startSquare, targetSquare);
            if (moveToUnmake.captured) {
                //CAPTURE
                addToBitboards(board, moveToUnmake.capturePosPlace, moveToUnmake.capturedPieceType, board->whiteToMove);
            }
            break;
        case EnPassantCaptureFlag:
            break;
        case CastleFlag:
            break;
        case DoublePush:
            replacePosToBitboards(board, startSquare, targetSquare);
            board->enPassantSquare = targetSquare;
            break;
        default:
            break;
    }
    board->whiteToMove = !board->whiteToMove;
    board->fullMoves--;
    movesMade.pop_back();
}

void unmakeMove(Board* board, Move move) {
    board->enPassantSquare = move.prevEnPassantSquare;
    if (move.prevEnPassantSquare != 0) {
        board->enPassantSquare |= 1ULL << 7;
    }
    switch (move.move_type) {
        case NORMAL_MOVE:
            replacePosToBitboards(board, move.newPos, move.oldPos);
            break;
        case CAPTURE:
            replacePosToBitboards(board, move.newPos, move.oldPos);
            addToBitboards(board, move.specialPiece, move.specialPieceType, !move.team);
            break;
        case DOUBLE_PAWN_PUSH:
            replacePosToBitboards(board, move.newPos, move.oldPos);
            break;
        case EN_PASSANT:
            replacePosToBitboards(board, move.newPos, move.oldPos);
            addToBitboards(board, move.specialPiece, move.specialPieceType, !move.team);
            break;
        case PROMOTION:
            removeFromBitboards(board, move.specialPiece);
            addToBitboards(board, move.oldPos, PAWN, move.team);
            break;
        case CAPTURE_PROMOTION:
            removeFromBitboards(board, move.specialPiece);
            addToBitboards(board, move.oldPos, PAWN, move.team);
            addToBitboards(board, move.specialPiece, move.specialPieceType, !move.team);
            break;
        case CASTLE:
            break;
    }
    board->whiteToMove = !board->whiteToMove;
    board->fullMoves--;
}
