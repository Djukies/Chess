#include "GameLogic.h"
#include <iostream>

GameLogic::GameLogic(Board *board) : board(board) {}

void GameLogic::undoMoves() {
    if (IsKeyPressed(KEY_LEFT)) {

        if (board->fullMoves - firstMadMove-1 < 0) {
            return;
        }
        unmakeMove(board, madeMoves[board->fullMoves - firstMadMove-1]);
        board->fullMoves -= 1;
    } else if (IsKeyPressed(KEY_RIGHT)) {
        if (board->fullMoves - firstMadMove < madeMoves.size()) {
            Move move = madeMoves[board->fullMoves - firstMadMove];
            makeMove(board, move);
        }
    }
}


bool GameLogic::tryMove(uint8_t oldPos, uint8_t newPos) {
    Move move = getMove(oldPos, newPos, moves);
    if (move.move_type != NO_MOVE) {
        makeMove(board, move);
        while (board->fullMoves - firstMadMove <= madeMoves.size()) {
            madeMoves.pop_back();
        }
        this->moves = this->calculateMoves();
        madeMoves.push_back(move);
        return true;
    } // Else
    return false;
}

void GameLogic::setOpponentControlledSquares() {
    board->opponentSquares = 0;
    std::map<integer, std::vector<Move>> possibleMoves;
    bit_board pieces;
    if (board->activeTeam == TEAMWHITE) {
        pieces = board->whitePieces;
    }
    else {
        pieces = board->blackPieces;
    }

    while (pieces) {
        integer i = __builtin_ctzll(pieces); // Find index of the least significant set bit
        bit_board mask = 1ULL << i;
        if (mask & board->pawns) {
            if (board->activeTeam == TEAMWHITE) {
                possibleMoves[i].push_back({board->enPassantSquare, CAPTURE, i, (integer) (i + 7), PAWN, board->activeTeam});
                possibleMoves[i].push_back({board->enPassantSquare, CAPTURE, i, (integer) (i + 9), PAWN, board->activeTeam});
            } else {
                possibleMoves[i].push_back({board->enPassantSquare, CAPTURE, i, (integer) (i - 7), PAWN, board->activeTeam});
                possibleMoves[i].push_back({board->enPassantSquare, CAPTURE, i, (integer) (i - 9), PAWN, board->activeTeam});
            }
        } else if (mask & board->bishops) {
            possibleMoves[i] = calculateSlidingPieceMoves(i, {-9, -7, 7, 9});
        } else if (mask & board->knights) {
            possibleMoves[i] = calculateKnightMoves(i);
        } else if (mask & board->rooks) {
            possibleMoves[i] = calculateSlidingPieceMoves(i, {-8, -1, 1, 8});
        } else if (mask & board->queens) {
            possibleMoves[i] = calculateSlidingPieceMoves(i, {-9, -8, -7, -1, 1, 7, 8, 9});
        } else if (mask & board->kings) {
            possibleMoves[i] = calculateKingMoves(i, false);
        }
        pieces &= pieces - 1;
    }
    for (const auto &[fst, moveList]: possibleMoves) {
        for (Move opponentMove: moveList) {
            board->opponentSquares |= 1ULL << opponentMove.newPos;
        }
    }
}

std::map<integer, std::vector<Move>> GameLogic::calculateMoves(bool onlyLegalMoves) {
    bit_board pieces;
    std::map<integer, std::vector<Move>> possibleMoves;
    if (onlyLegalMoves) {
        board->activeTeam = !board->activeTeam;
        this->setOpponentControlledSquares();
        board->activeTeam = !board->activeTeam;
    }
    if (board->activeTeam == TEAMWHITE) {
        pieces = board->whitePieces;
    }
    else {
        pieces = board->blackPieces;
    }

    while (pieces) {
        integer i = __builtin_ctzll(pieces); // Find index of the least significant set bit
        bit_board mask = 1ULL << i;
        if (mask & board->pawns) {
            possibleMoves[i] = calculatePawnMoves(i);
        } else if (mask & board->bishops) {
            possibleMoves[i] = calculateSlidingPieceMoves(i, {-9, -7, 7, 9});
        } else if (mask & board->knights) {
            possibleMoves[i] = calculateKnightMoves(i);
        } else if (mask & board->rooks) {
            possibleMoves[i] = calculateSlidingPieceMoves(i, {-8, -1, 1, 8});
        } else if (mask & board->queens) {
            possibleMoves[i] = calculateSlidingPieceMoves(i, {-9, -8, -7, -1, 1, 7, 8, 9});
        } else if (mask & board->kings) {
            possibleMoves[i] = calculateKingMoves(i, onlyLegalMoves);
        }
        if (onlyLegalMoves) {
            std::vector<Move> pieceMoves = possibleMoves[i];
            for (Move move: pieceMoves) {
                if (mask & board->kings) {
                    if (move.move_type == NORMAL_MOVE) {
                        //continue;
                    }
                }
                else if (!((1ULL << move.oldPos) & board->opponentSquares)) {
                    if (board->activeTeam == TEAMWHITE) {
                        if (!((1ULL << board->whiteKingPos) & board->opponentSquares)) {
                            continue;
                        }
                    } else {
                        if (!((1ULL << board->blackKingPos) & board->opponentSquares)) {
                            continue;
                        }
                    }
                }
                makeMove(board, move);
                bit_board pieceBitboard;
                integer king;
                if (board->activeTeam == TEAMWHITE) {
                    pieceBitboard = board->whitePieces;
                    king = board->blackKingPos;
                } else {
                    pieceBitboard = board->blackPieces;
                    king = board->whiteKingPos;
                }
                std::map<integer, std::vector<Move>> opponentMoves = calculateMoves(false);
                while (pieceBitboard) {
                    int index = __builtin_ctzll(pieceBitboard); // Find index of the least significant set bit

                    if (getMove(index, king, opponentMoves).move_type != NO_MOVE) {
                        auto it = std::find(possibleMoves[i].begin(), possibleMoves[i].end(), move);

                        if (it != pieceMoves.end()) { // Check if the item was found
                            possibleMoves[i].erase(it); // Remove the item at the found position
                            break;
                        }
                    }
                    pieceBitboard &= pieceBitboard - 1;
                }

                unmakeMove(board, move);
            }
        }
        pieces &= pieces - 1; // Remove the least significant set bit
    }


    return possibleMoves;
}

Piece getPiece(Board* board, integer pos) {
    bit_board mask = 1ULL << pos;
    if (mask & board->pawns) return PAWN;
    if (mask & board->bishops) return BISHOP;
    if (mask & board->knights) return KNIGHT;
    if (mask & board->rooks) return ROOK;
    if (mask & board->queens) return QUEEN;
    if (mask & board->kings) return KING;
    return NONE;
}

bool getTeam(Board* board, integer pos) {
    bit_board mask = 1ULL << pos;
    if (mask & board->whitePieces) return TEAMWHITE;
    return TEAMBLACK;
}

int8_t numOfSquaresToEdge(integer startSquare, int8_t dir) {
    Vector2 startSquareVector = intToVector2(startSquare);
    switch (dir) {
        case -9: // Diagonal up-left
            return std::min((int8_t) startSquareVector.x, (int8_t) startSquareVector.y);
        case -8: // Up
            return (int8_t) startSquareVector.y;
        case -7: // Diagonal up-right
            return std::min((int8_t) (7 - startSquareVector.x), (int8_t) startSquareVector.y);
        case -1: // Left
            return (int8_t) startSquareVector.x;
        case 1: // Right
            return (int8_t) (7 - startSquareVector.x);
        case 7: // Diagonal down-left
            return std::min((int8_t) startSquareVector.x, (int8_t) (7 - startSquareVector.y));
        case 8: // Down
            return (int8_t) (7 - startSquareVector.y);
        case 9: // Diagonal down-right
            return std::min((int8_t) (7 - startSquareVector.x), (int8_t) (7 - startSquareVector.y));
        default:
            throw std::invalid_argument("Invalid direction");
    }
}

bool isValidKnightMove(integer pos, int8_t dir) {
    if (!(0 <= pos && pos < 64)) {
        return false;
    }
    integer newPos = pos + dir;
    if (!(0 <= newPos && newPos < 64)) {
        return false;
    }
    integer currentFile = intToY(pos);
    integer currentRank = intToX(pos);

    integer newFile = intToY(pos + dir);
    integer newRank = intToX(pos + dir);
    if (std::abs(currentRank - newRank) > 2)
    {
        return false;
    }
    if (std::abs(currentFile - newFile) > 2)
    {
        return false;
    }
    return true;
}

bool isValidKingMove(integer pos, int8_t dir) {
    if (!(0 <= pos && pos < 64)) {
        return false;
    }
    integer newPos = pos + dir;
    if (!(0 <= newPos && newPos < 64)) {
        return false;
    }
    integer currentFile = intToY(pos);
    integer currentRank = intToX(pos);

    integer newFile = intToY(pos + dir);
    integer newRank = intToX(pos + dir);
    if (std::abs(currentRank - newRank) > 1)
    {
        return false;
    }
    if (std::abs(currentFile - newFile) > 1)
    {
        return false;
    }
    return true;
}

std::vector<Move> GameLogic::calculatePawnMoves(integer piece) {
    std::vector<Move> pieceMoves;

    bit_board allPieces = board->blackPieces | board->whitePieces;
    bit_board pieceBitBoard = 1ULL << piece;
    bit_board forward = 1ULL << (piece + (board->activeTeam == TEAMWHITE ? 8 : -8));
    forward &= ~allPieces;
    integer newPos;
    integer newY;
    if (forward) {
        newPos = piece + (board->activeTeam == TEAMWHITE ? 8 : -8);
        newY = intToY(newPos);
        if (newY == 7 || newY == 0) {
            pieceMoves.push_back({board->enPassantSquare, PROMOTION, piece, newPos, PAWN, board->activeTeam, newPos, QUEEN, QUEEN});
        } else {
            pieceMoves.push_back({board->enPassantSquare, NORMAL_MOVE, piece, newPos});
            bit_board pawnSpawns = 0xFFULL << (board->activeTeam == TEAMWHITE ? 8 : 48);
            if (pieceBitBoard & pawnSpawns) {
                forward = 1ULL << (piece + (board->activeTeam == TEAMWHITE ? 16 : -16));
                forward &= ~allPieces;
                if (forward) {
                    pieceMoves.push_back({board->enPassantSquare, DOUBLE_PAWN_PUSH, piece, (integer) (piece + (board->activeTeam == TEAMWHITE ? 16 : -16))});
                }
            }
        }
    }
    if (piece % 8 - 1 >= 0) { // Can go left
        newPos = piece + (board->activeTeam == TEAMWHITE ? 7 : -9);
        newY = intToY(newPos);

        bit_board left = 1ULL << newPos;
        left &= board->activeTeam == TEAMWHITE ? board->blackPieces : board->whitePieces;
        if (left) {
            if (newY == 7 || newY == 0) {
                pieceMoves.push_back({board->enPassantSquare, CAPTURE_PROMOTION, piece, newPos, PAWN, board->activeTeam, newPos, getPiece(board, newPos), QUEEN});
            } else {
                pieceMoves.push_back({board->enPassantSquare, CAPTURE, piece, newPos, PAWN, board->activeTeam, newPos, getPiece(board, newPos)});
            }
        } else if (board->enPassantSquare & (1ULL << 7)) {
            if (piece - 1 ==
                (board->enPassantSquare & ~(1ULL << 7))) { // Checks if the square next to it is the enPassantSquare
                pieceMoves.push_back({board->enPassantSquare, EN_PASSANT, piece, newPos, PAWN, board->activeTeam, (integer) (piece - 1), PAWN});
            }
        }
    }
    if (piece % 8 + 1 < 8) { // Can go right
        newPos = piece + (board->activeTeam == TEAMWHITE ? 9 : -7);
        newY = intToY(newPos);

        bit_board right = 1ULL << newPos;
        right &= board->activeTeam == TEAMWHITE ? board->blackPieces : board->whitePieces;
        if (right) {
            if (newY == 7 || newY == 0) {
                pieceMoves.push_back({board->enPassantSquare, CAPTURE_PROMOTION, piece, newPos, PAWN, board->activeTeam, newPos, getPiece(board, newPos), QUEEN});
            } else {
                pieceMoves.push_back({board->enPassantSquare, CAPTURE, piece, newPos, PAWN, board->activeTeam, newPos, getPiece(board, newPos)});
            }
        } else if (board->enPassantSquare & (1ULL << 7)) {
            if (piece + 1 ==
                (board->enPassantSquare & ~(1ULL << 7))) { // Checks if the square next to it is the enPassantSquare
                pieceMoves.push_back({board->enPassantSquare, EN_PASSANT, piece, newPos, PAWN, board->activeTeam, (integer) (piece + 1), PAWN});
            }
        }

    }

    return pieceMoves;
}

std::vector<Move> GameLogic::calculateKnightMoves(integer piece) {
    std::vector<Move> pieceMoves;
    std::vector<int8_t> dirs = {-17, -15, -10, -6, 6, 10, 15, 17};
    for (int8_t dir : dirs) {
        if (!isValidKnightMove(piece, dir)) {
            continue;
        }

        bit_board pieceBitBoard = 1ULL << piece;
        bool pieceTeam = pieceBitBoard & board->whitePieces ? TEAMWHITE : TEAMBLACK;
        if (dir > 0) pieceBitBoard <<= dir;
        else pieceBitBoard >>= -dir;
        if (!(pieceBitBoard & (pieceTeam == TEAMWHITE ? board->whitePieces : board->blackPieces))) {
            if (pieceBitBoard & (board->whitePieces | board->blackPieces)) {
                pieceMoves.push_back({board->enPassantSquare, CAPTURE, piece, (integer)(piece + dir), KNIGHT, board->activeTeam, (integer)(piece + dir), getPiece(board, (piece + dir))});
            } else {
                pieceMoves.push_back({board->enPassantSquare, NORMAL_MOVE, piece, (integer) (piece + dir)});
            }
        }

    }
    return pieceMoves;
}

std::vector<Move> GameLogic::calculateKingMoves(integer piece, bool onlyLegalMoves) {
    std::vector<Move> pieceMoves;
    std::vector<int8_t> dirs = {-9, -8, -7, -1, 1, 7, 8, 9};
    for (int8_t dir : dirs) {
        if (!isValidKingMove(piece, dir)) {
            continue;
        }

        bit_board pieceBitBoard = 1ULL << piece;
        bool pieceTeam = pieceBitBoard & board->whitePieces ? TEAMWHITE : TEAMBLACK;
        if (dir > 0) pieceBitBoard <<= dir;
        else pieceBitBoard >>= -dir;
        if (!(pieceBitBoard & (pieceTeam == TEAMWHITE ? board->whitePieces : board->blackPieces))) {
            if (pieceBitBoard & (board->whitePieces | board->blackPieces)) {
                pieceMoves.push_back({board->enPassantSquare, CAPTURE, piece, (integer) (piece + dir), KING, board->activeTeam, (integer) (piece + dir), getPiece(board, (piece + dir))});
            } else {
                if (onlyLegalMoves) {
                    if (1ULL << (piece + dir) & board->opponentSquares) {
                        continue;
                    }
                }
                pieceMoves.push_back({board->enPassantSquare, NORMAL_MOVE, piece, (integer) (piece + dir)});
            }
        }

    }
    return pieceMoves;
}

std::vector<Move> GameLogic::calculateSlidingPieceMoves(integer piece, std::vector<int8_t> dirs) {
    std::vector<Move> pieceMoves;
    for (int8_t dir : dirs) {
        bit_board pieceBitBoard = 1ULL << piece;
        bool pieceTeam = pieceBitBoard & board->whitePieces ? TEAMWHITE : TEAMBLACK;
        for (int8_t i = 1; i <= numOfSquaresToEdge(piece, dir); i++) {
            if (dir > 0) pieceBitBoard <<= dir;
            else pieceBitBoard >>= -dir;
            if (pieceTeam == TEAMWHITE) {
                if (pieceBitBoard & board->blackPieces) {
                    pieceMoves.push_back({board->enPassantSquare, CAPTURE, piece, (integer)(piece + (dir * i)), getPiece(board, piece), board->activeTeam, (integer)(piece + (dir * i)), getPiece(board, piece + (dir * i))});
                    break;
                } else if (pieceBitBoard & board->whitePieces) {
                    break;
                }
                pieceMoves.push_back({board->enPassantSquare, NORMAL_MOVE, piece, (integer)(piece + (dir * i))});
            } else {
                if (pieceBitBoard & board->whitePieces) {
                    pieceMoves.push_back({board->enPassantSquare, CAPTURE, piece, (integer)(piece + (dir * i)), getPiece(board, piece), board->activeTeam, (integer)(piece + (dir * i)), getPiece(board, piece + (dir * i))});
                    break;
                } else if (pieceBitBoard & board->blackPieces) {
                    break;
                }
                pieceMoves.push_back({board->enPassantSquare, NORMAL_MOVE, piece, (integer)(piece + (dir * i))});
            }
        }
    }
    return pieceMoves;
}