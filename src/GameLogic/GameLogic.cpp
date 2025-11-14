#include "GameLogic.h"
#include "MoveGeneration/MoveGeneration.h"
#include <iostream>

GameLogic::GameLogic(Board *board, Algorithm* algorithmWhite, Algorithm* algorithmBlack) : board(board), algorithmWhite(algorithmWhite), algorithmBlack(algorithmBlack) {
    Zobrist::init();
    board->zobristKey = computeHash();
}

uint64_t GameLogic::computeHash() const {
    uint64_t hash = 0;

    // For each piece on board (example)
    for (int sq = 0; sq < 64; ++sq) {
        int piece = getPiece(board, sq) + (containsSquare(board->whitePieces, sq) ? 0 : 5); // e.g. 0-11 or -1 if empty
        if (piece != 12)
            hash ^= Zobrist::pieceSquare[piece][sq];
    }

    // Side to move
    if (board->whiteToMove)
        hash ^= Zobrist::sideToMove;

    // Castling rights
    if (containsSquare(board->castleRights, 0)) hash ^= Zobrist::castling[0];
    if (containsSquare(board->castleRights, 1)) hash ^= Zobrist::castling[1];
    if (containsSquare(board->castleRights, 2)) hash ^= Zobrist::castling[2];
    if (containsSquare(board->castleRights, 3)) hash ^= Zobrist::castling[3];

    // En
    if (board->enPassantSquare != 0) {
        int epFile = intToX(board->enPassantSquare); // 0..7 or -1
        hash ^= Zobrist::enPassantFile[epFile];
    }
    std::cout << hash << std::endl;
    return hash;
}

bool GameLogic::isThreeFoldRepetition() const {
    int count = 0;
    for (uint64_t hash : board->history) {
        if (hash == board->zobristKey)
            ++count;
        if (count >= 2)
            return true;
    }
    return false;
}



void GameLogic::undoMoves() {
    if (!IsKeyPressed(KEY_LEFT_SHIFT)) {
        if (IsKeyPressed(KEY_LEFT)) {

            if (board->fullMoves - firstMadMove <= 0) {
                return;
            }
            unMakeMove(board, madeMoves[board->fullMoves - firstMadMove - 1]);
            board->movesVector = calculateLegalMoves(board);
            board->movesMap = moveVectorToMap(board->movesVector);
        } else if (IsKeyPressed(KEY_RIGHT)) {
            if (board->fullMoves - firstMadMove < madeMoves.size()) {
                Move move = madeMoves[board->fullMoves - firstMadMove];
                makeMove(board, move);
                board->movesVector = calculateLegalMoves(board);
                board->movesMap = moveVectorToMap(board->movesVector);
            }
        }
    }
}


bool GameLogic::tryMove(uint8_t oldPos, uint8_t newPos) {
    Move move = getMove(oldPos, newPos, board->movesMap);
    if (move != 0) {
        if ((move & flagMask) >> 12 >= 4) {
            std::cout << "PROMOTING" << std::endl;
        }
        makeMove(board, move);
        board->zobristKey = computeHash();
        if (isThreeFoldRepetition()) {
            std::cout << "REPETIZING" << std::endl;
        }
        board->history.push_back(board->zobristKey);
        board->movesVector = calculateLegalMoves(board);
        board->movesMap = moveVectorToMap(board->movesVector);
        if (board->fullMoves-firstMadMove > madeMoves.size()) {
            madeMoves.push_back(move);
            /*if (board->whiteToMove && board->algorithmWhite) {
                this->letAlgoMakeMove();
            } else if ((!board->whiteToMove) && board->algorithmBlack) {
                this->letAlgoMakeMove();
            }*/
            return true;
        }

        if (move != madeMoves[board->fullMoves - firstMadMove - 1]) {
            uint size = madeMoves.size();
            for (int i = board->fullMoves - firstMadMove - 1; i < size; i++) {
                madeMoves.pop_back();
            }
            madeMoves.push_back(move);
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



void GameLogic::letAlgoMakeMove() {
    if (board->whiteToMove) {
        if (algoWhiteStockfish) {
            letAlgoStockfishMakeMove();
            return;
        }
        if (algorithmWhite->bestMove == 0) {
            algorithmWhite->calcBestMove(4, TEAMWHITE);
        }
    } else {
        if (algoBlackStockfish) {
            letAlgoStockfishMakeMove();
            return;
        }
        if (algorithmBlack->bestMove == 0) {
            algorithmBlack->calcBestMove(4, TEAMBLACK);
        }
    }
    makeMove(board, board->whiteToMove ? algorithmWhite->bestMove : algorithmBlack->bestMove);
    board->movesVector = calculateLegalMoves(board);
    board->movesMap = moveVectorToMap(board->movesVector);
    Move move = board->whiteToMove ? algorithmWhite->bestMove : algorithmBlack->bestMove;
    if (board->fullMoves-firstMadMove > madeMoves.size()) {
        madeMoves.push_back(move);
        if (board->whiteToMove) {
            algorithmWhite->calcBestMove(4, TEAMWHITE);
        } else {
            algorithmBlack->calcBestMove(4, TEAMBLACK);
        }
        calculateLegalMoves(board);
        if (board->checkMate || board->staleMate) {
            std::cout << "GAME ENDED" << std::endl;

            return;
        }
        return;
    }

    if (move != madeMoves[board->fullMoves - firstMadMove - 1]) {
        uint size = madeMoves.size();
        for (int i = board->fullMoves - firstMadMove - 1; i < size; i++) {
            madeMoves.pop_back();
        }
        madeMoves.push_back(move);
    }
    if (board->whiteToMove) {
        if (algoWhiteStockfish) return;
        algorithmWhite->calcBestMove(4, TEAMWHITE);
    } else {
        if (algoBlackStockfish) return;
        algorithmBlack->calcBestMove(4, TEAMBLACK);
    }
}

void GameLogic::letAlgoStockfishMakeMove() {
    std::string move = StockfishGetMove(board->movesMade, board->startingFen);
    integer from = charToInt(move.substr(0, 2));
    integer to = charToInt(move.substr(2, 2));
    std::cout << tryMove(from, to) << std::endl;
}



std::map<integer, std::vector<Move>> GameLogic::moveVectorToMap(std::vector<Move> movesToRework) {
    std::map<integer, std::vector<Move>> movesToReturn;

    for (Move move : movesToRework) {
        movesToReturn[move & startSquareMask].push_back(move);
    }

    return movesToReturn;
}