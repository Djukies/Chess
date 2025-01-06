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

std::vector<MadeMove> movesMade = {};

void makeSmallMove(Board* board, Move Move) {
    integer startSquare = Move & startSquareMask;
    integer targetSquare = (Move & targetSquareMask) >> 6;
    int flag = ((Move & flagMask) >> 12);
    MadeMove madeMove;
    madeMove.prevEnPassant = board->enPassantSquare;
    board->enPassantSquare = 0;
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
            madeMove.captured = true;
            madeMove.capturedPieceType = PAWN;
            madeMove.capturePosPlace = targetSquare + (board->whiteToMove ? 8 : -8);
            replacePosToBitboards(board, startSquare, targetSquare);
            removeFromBitboards(board, targetSquare + (board->whiteToMove ? 8 : -8));
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

void unMakeSmallMove(Board* board, Move Move) {
    integer targetSquare = Move & startSquareMask;
    integer startSquare = (Move & targetSquareMask) >> 6;
    int flag = ((Move & flagMask) >> 12);
    MadeMove moveToUnmake = movesMade[movesMade.size()-1];
    board->enPassantSquare = moveToUnmake.prevEnPassant;
    switch (flag) {
        case NoFlag:
            replacePosToBitboards(board, startSquare, targetSquare);
            if (moveToUnmake.captured) {
                //CAPTURE
                addToBitboards(board, moveToUnmake.capturePosPlace, moveToUnmake.capturedPieceType, board->whiteToMove);
            }
            break;
        case EnPassantCaptureFlag:
            replacePosToBitboards(board, startSquare, targetSquare);
            addToBitboards(board, moveToUnmake.capturePosPlace, moveToUnmake.capturedPieceType, board->whiteToMove);
            break;
        case CastleFlag:
            break;
        case DoublePush:
            replacePosToBitboards(board, startSquare, targetSquare);
            break;
        default:
            break;
    }
    board->whiteToMove = !board->whiteToMove;
    board->fullMoves--;
    movesMade.pop_back();
}