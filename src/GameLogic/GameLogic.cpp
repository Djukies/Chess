#include "GameLogic.h"
#include "MoveGeneration/MoveGeneration.h"
#include <iostream>

GameLogic::GameLogic(Board *board) : board(board) {}

void GameLogic::undoMoves() {
    if (IsKeyPressed(KEY_LEFT)) {

        if (board->fullMoves - firstMadMove <= 0) {
            return;
        }
        unMakeSmallMove(board, madeMoves[board->fullMoves - firstMadMove-1]);
        board->movesVector = calculateLegalMoves(board);
        board->movesMap = moveVectorToMap(board->movesVector);
    } else if (IsKeyPressed(KEY_RIGHT)) {
        if (board->fullMoves - firstMadMove < madeMoves.size()) {
            small_move move = madeMoves[board->fullMoves - firstMadMove];
            makeSmallMove(board, move);
            board->movesVector = calculateLegalMoves(board);
            board->movesMap = moveVectorToMap(board->movesVector);
        }
    }
}


bool GameLogic::tryMove(uint8_t oldPos, uint8_t newPos) {
    small_move smallMove = getSmallMove(oldPos, newPos, board->movesMap);
    if (smallMove != 0) {
        makeSmallMove(board, smallMove);

        board->movesVector = calculateLegalMoves(board);
        board->movesMap = moveVectorToMap(board->movesVector);
        if (board->fullMoves-firstMadMove > madeMoves.size()) {
            madeMoves.push_back(smallMove);
            return true;
        }

        if (smallMove != madeMoves[board->fullMoves - firstMadMove - 1]) {
            uint size = madeMoves.size();
            for (int i = board->fullMoves - firstMadMove - 1; i < size; i++) {
                madeMoves.pop_back();
            }
            madeMoves.push_back(smallMove);
        }
        return true;
    } /*else {
        Move move = getMove(oldPos, newPos, moves);
        if (move.move_type != NO_MOVE) {
            makeMove(board, move);
            while (board->fullMoves - firstMadMove <= madeMoves.size()) {
                madeMoves.pop_back();
            }
            //board->whiteToMove = TEAMWHITE;
            this->moves = this->calculateMoves();
            madeMoves.push_back(move);
            board->movesVector = calculateLegalMoves(board);
            board->movesMap = moveVectorToMap(board->movesVector);
            return true;
        } // Else
        return false;
    }*/ return false;
}

void GameLogic::setOpponentControlledSquares() {
    board->opponentSquares = 0;
    std::map<integer, std::vector<Move>> possibleMoves;
    bit_board pieces;
    if (board->whiteToMove == TEAMWHITE) {
        pieces = board->whitePieces;
    }
    else {
        pieces = board->blackPieces;
    }

    while (pieces) {
        integer i = __builtin_ctzll(pieces); // Find index of the least significant set bit
        bit_board mask = 1ULL << i;
        if (mask & board->pawns) {
            if (board->whiteToMove == TEAMWHITE) {
                possibleMoves[i].push_back({board->enPassantSquare, CAPTURE, i, (integer) (i + 7), PAWN, board->whiteToMove});
                possibleMoves[i].push_back({board->enPassantSquare, CAPTURE, i, (integer) (i + 9), PAWN, board->whiteToMove});
            } else {
                possibleMoves[i].push_back({board->enPassantSquare, CAPTURE, i, (integer) (i - 7), PAWN, board->whiteToMove});
                possibleMoves[i].push_back({board->enPassantSquare, CAPTURE, i, (integer) (i - 9), PAWN, board->whiteToMove});
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
        if (board->whiteToMove == TEAMWHITE) {
            board->whitePieces &= ~(1ULL << board->whiteKingPos);
        } else {
            board->blackPieces &= ~(1ULL << board->blackKingPos);
        }
        board->whiteToMove = !board->whiteToMove;
        this->setOpponentControlledSquares();
        board->whiteToMove = !board->whiteToMove;
        if (board->whiteToMove == TEAMWHITE) {
            board->whitePieces |= 1ULL << board->whiteKingPos;
        } else {
            board->blackPieces |= 1ULL << board->blackKingPos;
        }
    }
    int x;
    int y;
    if (board->whiteToMove == TEAMWHITE) {
        pieces = board->whitePieces;
        x = intToX(board->whiteKingPos);
        y = intToY(board->whiteKingPos);
    }
    else {
        pieces = board->blackPieces;
        x = intToX(board->blackKingPos);
        y = intToY(board->blackKingPos);
    }


    bit_board kingY = (uint64_t )0xFF << (y*8);
    bit_board kingX = 0x0101010101010101ULL << x;

    bit_board leftSide = ((1ULL << x) - 1) * 0x0101010101010101ULL;
    bit_board rightSide = ~(((1ULL << (x+1)) - 1) * 0x0101010101010101ULL);
    bit_board upSide = y < 7 ? UINT64_MAX << (8 * (y+1)) : 0ULL;
    bit_board downSide =  y > 0 ? UINT64_MAX >> (8 * (8 - y)) : 0ULL;

    bit_board kingDiagonalTopLeft = x>y ? 0x8040201008040201 >> ((x-y)*8) : 0x8040201008040201 << ((y-x)*8);
    bit_board kingDiagonalTopRight = (7-x) > y ? 0x0102040810204080 >> (((7-x)-y)*8) : 0x0102040810204080 << ((y-(7-x))*8);

    bit_board left = kingY & leftSide;
    bit_board right = kingY & rightSide;
    bit_board up = kingX & upSide;
    bit_board down = kingX & downSide;

    bit_board topLeft = kingDiagonalTopLeft & leftSide;
    bit_board topRight = kingDiagonalTopRight & rightSide;
    bit_board bottomLeft = kingDiagonalTopRight & leftSide;
    bit_board bottomRight = kingDiagonalTopLeft & rightSide;

    bit_board straights = left | right | up | down;
    bit_board diagonals = topLeft | topRight | bottomLeft | bottomRight;
    bit_board allPinDirs = left | right | up | down | topLeft | topRight | bottomLeft | bottomRight;

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
                        continue;
                    }
                } else if (!((1ULL << move.oldPos) & (allPinDirs & board->opponentSquares))) {
                    if (board->whiteToMove == TEAMWHITE) {
                        if (!((1ULL << board->whiteKingPos) & board->opponentSquares)) {
                            continue;
                        }
                    } else {
                        if (!((1ULL << board->blackKingPos) & board->opponentSquares)) {
                            continue;
                        }
                    }
                } else if ((1ULL << move.oldPos) & allPinDirs) {
                    bit_board oldPosBitboard = 1ULL << move.oldPos;
                    bit_board opponentDiagonalSliders = board->bishops | board->queens;
                    bit_board opponentStraightSliders = board->rooks | board->queens;
                    if (board->whiteToMove == TEAMWHITE) {
                        opponentDiagonalSliders &= board->blackPieces;
                        opponentStraightSliders &= board->blackPieces;
                    } else {
                        opponentDiagonalSliders &= board->whitePieces;
                        opponentStraightSliders &= board->whitePieces;
                    }
                }
                makeMove(board, move);
                bit_board pieceBitboard;
                integer king;
                if (board->whiteToMove == TEAMWHITE) {
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

/*inline Piece getPiece(Board* board, integer pos) {
    bit_board mask = 1ULL << pos;
    if (mask & board->pawns) return PAWN;
    if (mask & board->bishops) return BISHOP;
    if (mask & board->knights) return KNIGHT;
    if (mask & board->rooks) return ROOK;
    if (mask & board->queens) return QUEEN;
    if (mask & board->kings) return KING;
    return NONE;
}*/

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
    bit_board forward = 1ULL << (piece + (board->whiteToMove == TEAMWHITE ? 8 : -8));
    forward &= ~allPieces;
    integer newPos;
    integer newY;
    if (forward) {
        newPos = piece + (board->whiteToMove == TEAMWHITE ? 8 : -8);
        newY = intToY(newPos);
        if (newY == 7 || newY == 0) {
            pieceMoves.push_back({board->enPassantSquare, PROMOTION, piece, newPos, PAWN, board->whiteToMove, newPos, QUEEN, QUEEN});
        } else {
            pieceMoves.push_back({board->enPassantSquare, NORMAL_MOVE, piece, newPos});
            bit_board pawnSpawns = 0xFFULL << (board->whiteToMove == TEAMWHITE ? 8 : 48);
            if (pieceBitBoard & pawnSpawns) {
                forward = 1ULL << (piece + (board->whiteToMove == TEAMWHITE ? 16 : -16));
                forward &= ~allPieces;
                if (forward) {
                    pieceMoves.push_back({board->enPassantSquare, DOUBLE_PAWN_PUSH, piece, (integer) (piece + (board->whiteToMove == TEAMWHITE ? 16 : -16))});
                }
            }
        }
    }
    if (piece % 8 - 1 >= 0) { // Can go left
        newPos = piece + (board->whiteToMove == TEAMWHITE ? 7 : -9);
        newY = intToY(newPos);

        bit_board left = 1ULL << newPos;
        left &= board->whiteToMove == TEAMWHITE ? board->blackPieces : board->whitePieces;
        if (left) {
            if (newY == 7 || newY == 0) {
                pieceMoves.push_back({board->enPassantSquare, CAPTURE_PROMOTION, piece, newPos, PAWN, board->whiteToMove, newPos, getPiece(board, newPos), QUEEN});
            } else {
                pieceMoves.push_back({board->enPassantSquare, CAPTURE, piece, newPos, PAWN, board->whiteToMove, newPos, getPiece(board, newPos)});
            }
        } else if (board->enPassantSquare & (1ULL << 7)) {
            if (piece - 1 ==
                (board->enPassantSquare & ~(1ULL << 7))) { // Checks if the square next to it is the enPassantSquare
                pieceMoves.push_back({board->enPassantSquare, EN_PASSANT, piece, newPos, PAWN, board->whiteToMove, (integer) (piece - 1), PAWN});
            }
        }
    }
    if (piece % 8 + 1 < 8) { // Can go right
        newPos = piece + (board->whiteToMove == TEAMWHITE ? 9 : -7);
        newY = intToY(newPos);

        bit_board right = 1ULL << newPos;
        right &= board->whiteToMove == TEAMWHITE ? board->blackPieces : board->whitePieces;
        if (right) {
            if (newY == 7 || newY == 0) {
                pieceMoves.push_back({board->enPassantSquare, CAPTURE_PROMOTION, piece, newPos, PAWN, board->whiteToMove, newPos, getPiece(board, newPos), QUEEN});
            } else {
                pieceMoves.push_back({board->enPassantSquare, CAPTURE, piece, newPos, PAWN, board->whiteToMove, newPos, getPiece(board, newPos)});
            }
        } else if (board->enPassantSquare & (1ULL << 7)) {
            if (piece + 1 ==
                (board->enPassantSquare & ~(1ULL << 7))) { // Checks if the square next to it is the enPassantSquare
                pieceMoves.push_back({board->enPassantSquare, EN_PASSANT, piece, newPos, PAWN, board->whiteToMove, (integer) (piece + 1), PAWN});
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
                pieceMoves.push_back({board->enPassantSquare, CAPTURE, piece, (integer)(piece + dir), KNIGHT, board->whiteToMove, (integer)(piece + dir), getPiece(board, (piece + dir))});
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
                pieceMoves.push_back({board->enPassantSquare, CAPTURE, piece, (integer) (piece + dir), KING, board->whiteToMove, (integer) (piece + dir), getPiece(board, (piece + dir))});
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
                    pieceMoves.push_back({board->enPassantSquare, CAPTURE, piece, (integer)(piece + (dir * i)), getPiece(board, piece), board->whiteToMove, (integer)(piece + (dir * i)), getPiece(board, piece + (dir * i))});
                    break;
                } else if (pieceBitBoard & board->whitePieces) {
                    break;
                }
                pieceMoves.push_back({board->enPassantSquare, NORMAL_MOVE, piece, (integer)(piece + (dir * i))});
            } else {
                if (pieceBitBoard & board->whitePieces) {
                    pieceMoves.push_back({board->enPassantSquare, CAPTURE, piece, (integer)(piece + (dir * i)), getPiece(board, piece), board->whiteToMove, (integer)(piece + (dir * i)), getPiece(board, piece + (dir * i))});
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

std::map<integer, std::vector<small_move>> GameLogic::moveVectorToMap(std::vector<small_move> movesToRework) {
    std::map<integer, std::vector<small_move>> movesToReturn;

    for (small_move move : movesToRework) {
        movesToReturn[move & startSquareMask].push_back(move);
    }

    return movesToReturn;
}