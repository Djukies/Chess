#include "Board.h"

#include <algorithm>

#include "Zobrist.h"
#include "Common/MoveGeneration/MoveGeneration.h"


Board* loadBoardFromFEN(const std::string fen) {
    auto *board = new Board;
    board->startingFen = fen;
    loadAllBoard(board, fen);

    return board;
}

void replacePosToBitboards(Board* board, integer oldPos, integer newPos) {
    bitboard oldPosMask = 1ULL << oldPos;
    bitboard newPosMask = 1ULL << newPos;

    bitboard mask = oldPosMask | newPosMask;


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
    bitboard mask = 1ULL << pos;
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
    bitboard mask = 1ULL << pos;
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

void promoteHelp(Board* board, MadeMove* made_move, integer startSquare, integer targetSquare) {
    if (containsSquare(board->blackPieces | board->whitePieces, targetSquare)) {
        made_move->captured = true;
        made_move->capturedPieceType = getPiece(board, targetSquare);
        made_move->capturePosPlace = targetSquare;
        removeFromBitboards(board, targetSquare);
    }
    removeFromBitboards(board, targetSquare);
    removeFromBitboards(board, startSquare);
}

std::vector<MadeMove> movesMade = {};

MadeMove makeMove(Board* board, Move move) {
    board->movesMade.push_back(move);
    integer startSquare = move & startSquareMask;
    integer targetSquare = (move & targetSquareMask) >> 6;
    int flag = ((move & flagMask) >> 12);
    MadeMove madeMove;
    madeMove.prevEnPassant = board->enPassantSquare;
    board->enPassantSquare = 0;
    int piece = getPiece(board, startSquare) + (containsSquare(startSquare, board->whitePieces) ? 0 : 6);
    board->zobristKey ^= Zobrist::pieceSquare[piece][startSquare];
    madeMove.prevCastleRights = board->castleRights;
    if (board->whiteToMove && (targetSquare == 56 || startSquare == 56)) {board->castleRights &= ~0b00000010ULL;}
    if (board->whiteToMove && (targetSquare == 63 || startSquare == 63)) {board->castleRights &= ~0b00000001ULL;}
    if (board->whiteToMove && (targetSquare == board->friendlyKingPos || startSquare == board->friendlyKingPos)) {board->castleRights &= ~0b00000011ULL;}
    if (!board->whiteToMove && (targetSquare == 0 || startSquare == 0)) {board->castleRights &= ~0b00001000ULL;}
    if (!board->whiteToMove && (targetSquare == 7 || startSquare == 7)) {board->castleRights &= ~0b00000100ULL;}
    if (!board->whiteToMove && (targetSquare == board->friendlyKingPos || startSquare == board->friendlyKingPos)) {board->castleRights &= ~0b00001100ULL;}
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
            replacePosToBitboards(board, startSquare, targetSquare);
            if (startSquare > targetSquare) {
                replacePosToBitboards(board, targetSquare-2, startSquare-1);
            } else {
                replacePosToBitboards(board, targetSquare+1, startSquare+1);
            }
            board->castleRights &= ~(board->whiteToMove ? 0b00000011ULL : 0b00001100ULL);

            break;
        case DoublePush:
            replacePosToBitboards(board, startSquare, targetSquare);
            board->enPassantSquare = targetSquare;
            break;
        case PromoteToBishopFlag:
            promoteHelp(board, &madeMove, startSquare, targetSquare);
            addToBitboards(board, targetSquare, BISHOP, board->whiteToMove);
            break;
        case PromoteToKnightFlag:
            promoteHelp(board, &madeMove, startSquare, targetSquare);
            addToBitboards(board, targetSquare, KNIGHT, board->whiteToMove);
            break;
        case PromoteToRookFlag:
            promoteHelp(board, &madeMove, startSquare, targetSquare);
            addToBitboards(board, targetSquare, ROOK, board->whiteToMove);
            break;
        case PromoteToQueenFlag:
            promoteHelp(board, &madeMove, startSquare, targetSquare);
            addToBitboards(board, targetSquare, QUEEN, board->whiteToMove);
            break;
        default:
            break;
    }

    board->whiteToMove = !board->whiteToMove;
    board->fullMoves++;
    movesMade.push_back(madeMove);
    return madeMove;
}

void unMakeMove(Board* board, Move move) {
    board->movesMade.pop_back();
    integer targetSquare = move & startSquareMask;
    integer startSquare = (move & targetSquareMask) >> 6;
    int flag = ((move & flagMask) >> 12);
    MadeMove moveToUnmake = movesMade[movesMade.size()-1];
    board->enPassantSquare = moveToUnmake.prevEnPassant;
    board->castleRights = moveToUnmake.prevCastleRights;
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
            replacePosToBitboards(board, startSquare, targetSquare);
            if (startSquare < targetSquare) {
                replacePosToBitboards(board, targetSquare-1, startSquare-2);
            } else {
                replacePosToBitboards(board, targetSquare+1, startSquare+1);
            }

            break;
        case DoublePush:
            replacePosToBitboards(board, startSquare, targetSquare);
            break;
        case PromoteToBishopFlag:
        case PromoteToKnightFlag:
        case PromoteToRookFlag:
        case PromoteToQueenFlag:
            removeFromBitboards(board, startSquare);
            addToBitboards(board, targetSquare, PAWN, !board->whiteToMove);
            if (moveToUnmake.captured) {
                //CAPTURE
                addToBitboards(board, moveToUnmake.capturePosPlace, moveToUnmake.capturedPieceType, board->whiteToMove);
            }
            break;
        default:
            break;
    }
    board->whiteToMove = !board->whiteToMove;
    board->fullMoves--;
    movesMade.pop_back();
}

Move getMove(integer oldPos, integer newPos, std::map<integer, std::vector<Move>> moves) {
    auto it = moves.find(oldPos); // Check if oldPos exists in the moves (Used to check if it is part of the right team for example)

    if (it != moves.end()) {
        auto found = std::find_if(it->second.begin(), it->second.end(), [newPos, oldPos](const Move &move) {
            return (move & startSquareMask) == oldPos && ((move & targetSquareMask) >> 6) == newPos;
        });
        if (found != it->second.end()) {
            return *found;
        }
    }
    return 0;
}

bool isThreeFoldRepetition(Board* board) {
    int count = 0;
    for (uint64_t hash : board->history) {
        if (hash == board->zobristKey)
            ++count;
        if (count >= 2)
            return true;
    }
    return false;
}

bool tryMove(Board* board, uint8_t oldPos, uint8_t newPos) {
    Move move = getMove(oldPos, newPos, board->movesMap);
    if (move != 0) {
        if ((move & flagMask) >> 12 >= 4) {
            std::cout << "PROMOTING" << std::endl;
        }
        makeMove(board, move);
        board->zobristKey = computeHash(board);
        if (isThreeFoldRepetition(board)) {
            std::cout << "REPETIZING" << std::endl;
        }
        board->history.push_back(board->zobristKey);
        board->movesVector = calculateLegalMoves(board);
        board->movesMap = moveVectorToMap(board->movesVector);
        if (board->fullMoves-board->firstMadMove > board->madeMoves.size()) {
            board->madeMoves.push_back(move);
            /*if (board->whiteToMove && board->algorithmWhite) {
                this->letAlgoMakeMove();
            } else if ((!board->whiteToMove) && board->algorithmBlack) {
                this->letAlgoMakeMove();
            }*/
            return true;
        }

        if (move != board->madeMoves[board->fullMoves - board->firstMadMove - 1]) {
            uint size = board->madeMoves.size();
            for (int i = board->fullMoves - board->firstMadMove - 1; i < size; i++) {
                board->madeMoves.pop_back();
            }
            board->madeMoves.push_back(move);
        }
        /*if (board->whiteToMove && board->algorithmWhite) {
            this->letAlgoMakeMove();
        } else if ((!board->whiteToMove) && board->algorithmBlack) {
            this->letAlgoMakeMove();
        }*/
        return true;
    }
    return false;
}