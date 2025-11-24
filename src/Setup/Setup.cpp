#include "Setup.h"

std::vector<std::string> separateFEN(const std::string FEN) {
    std::vector<std::string> separatedFEN = {};
    int spacePos = FEN.find(" ");
    separatedFEN.push_back(FEN.substr(0, spacePos));
    separatedFEN.push_back(FEN.substr(spacePos+1, 1));
    separatedFEN.push_back(FEN.substr(spacePos+3));


    spacePos = separatedFEN[CASTLE_RIGHTS].find(" ");
    separatedFEN.push_back(separatedFEN[CASTLE_RIGHTS].substr(spacePos+1));
    separatedFEN[CASTLE_RIGHTS] = separatedFEN[CASTLE_RIGHTS].substr(0, spacePos);

    spacePos = separatedFEN[EN_PASSANT_PLACE].find(" ");
    separatedFEN.push_back(separatedFEN[EN_PASSANT_PLACE].substr(spacePos+1));
    separatedFEN[EN_PASSANT_PLACE] = separatedFEN[EN_PASSANT_PLACE].substr(0, spacePos);

    spacePos = separatedFEN[HALF_MOVES].find(" ");
    separatedFEN.push_back(separatedFEN[HALF_MOVES].substr(spacePos+1));
    separatedFEN[HALF_MOVES] = separatedFEN[HALF_MOVES].substr(0, spacePos);

    return separatedFEN;
}


std::map<std::string, std::vector<uint8_t>> addFenToSetup(std::string boardFen)
{
    std::map<std::string, std::vector<uint8_t>> piecePositions;

    // Extract positions from the FEN string
    size_t rank = 0;
    size_t file = 0;

    for (char c : boardFen)
    {
        if (c == '/')
        {
            rank++;
            file = 0;
        }
        else if (isdigit(c))
        {
            file += (c - '0');
        }
        else
        {
            std::string piece;
            piece += c;
            size_t position = file + rank*8;
            piecePositions[piece].push_back(position);
            file++;
        }
    }

    return piecePositions;
}

void loadBoard(Board* board, const std::string boardFen) {
    const std::map<std::string, std::vector<uint8_t>>posForPieces =  addFenToSetup(boardFen);
    for (const auto &[piece, positions]: posForPieces) {
        if (piece == "P") {
            for (const int position: positions) {
                board->whitePieces |= 1ULL << position;
                board->pawns |= 1ULL << position;
            }
        } else if (piece == "p") {
            for (const int position: positions) {
                board->blackPieces |= 1ULL << position;
                board->pawns |= 1ULL << position;
            }
        } else if (piece == "N") {
            for (const int position: positions) {
                board->whitePieces |= 1ULL << position;
                board->knights |= 1ULL << position;
            }
        } else if (piece == "n") {
            for (const int position: positions) {
                board->blackPieces |= 1ULL << position;
                board->knights |= 1ULL << position;
            }
        } else if (piece == "B") {
            for (const int position: positions) {
                board->whitePieces |= 1ULL << position;
                board->bishops |= 1ULL << position;
            }
        } else if (piece == "b") {
            for (const int position: positions) {
                board->blackPieces |= 1ULL << position;
                board->bishops |= 1ULL << position;
            }
        } else if (piece == "R") {
            for (const int position: positions) {
                board->whitePieces |= 1ULL << position;
                board->rooks |= 1ULL << position;
            }
        } else if (piece == "r") {
            for (const int position: positions) {
                board->blackPieces |= 1ULL << position;
                board->rooks |= 1ULL << position;
            }
        } else if (piece == "Q") {
            for (const int position: positions) {
                board->whitePieces |= 1ULL << position;
                board->queens |= 1ULL << position;
            }
        } else if (piece == "q") {
            for (const int position: positions) {
                board->blackPieces |= 1ULL << position;
                board->queens |= 1ULL << position;
            }
        } else if (piece == "K") {
            for (const int position: positions) {
                board->whitePieces |= 1ULL << position;
                board->kings |= 1ULL << position;
                board->whiteKingPos = position;
            }
        } else if (piece == "k") {
            for (const int position: positions) {
                board->blackPieces |= 1ULL << position;
                board->kings |= 1ULL << position;
                board->blackKingPos = position;
            }
        } else {
            std::cout << "An Error occured during Board loading with piece " << piece << ", On positions:";
            for (int position: positions) {
                std::cout << " " << position;
            }
            std::cout << std::endl;
        }
    }
}

void loadActiveTeam(Board* board, const std::string activeTeamFen) {
    if (activeTeamFen == "w") board->whiteToMove = TEAMWHITE;
    else board->whiteToMove = TEAMBLACK;
}

void loadCastleRights(Board* board, const std::string castleRightsFen) {
    for (char c : castleRightsFen) {
        if (c == 'K') {
            board->castleRights |= 1ULL << 0;
        } else if (c == 'Q') {
            board->castleRights |= 1ULL << 1;
        } else if (c == 'k') {
            board->castleRights |= 1ULL << 2;
        } else if (c == 'q') {
            board->castleRights |= 1ULL << 3;
        }
    }
}

void loadEnPassantSquare(Board* board, const std::string enPassantSquareFen) {
    for (char c : enPassantSquareFen) {
        if (c == '-') {
            board->enPassantSquare = 0;
            break;
        } else if (std::isdigit(c)) { // Check if the character is a digit
            board->enPassantSquare += (c - '1')*8; // Convert the char to its numeric equivalent
        } else {
            switch (c) {
                case 'a':
                    board->enPassantSquare += 0;
                    break;
                case 'b':
                    board->enPassantSquare += 1;
                    break;
                case 'c':
                    board->enPassantSquare += 2;
                    break;
                case 'd':
                    board->enPassantSquare += 3;
                    break;
                case 'e':
                    board->enPassantSquare += 4;
                    break;
                case 'f':
                    board->enPassantSquare += 5;
                    break;
                case 'g':
                    board->enPassantSquare += 6;
                    break;
                case 'h':
                    board->enPassantSquare += 7;
                    break;
                default:
                    std::cout << "Error during enpassantsquare: " << c << std::endl;
                    break;
            }
        }
    }
}

void loadHalfMoves(Board* board, const std::string halfMovesFen) {
    for (char c : halfMovesFen) {
        if (isdigit(c)) {
            board->halfMoves *= 10;
            board->halfMoves += (c - '0');
        }
    }
}

void loadFullMoves(Board* board, const std::string fullMovesFen) {
    for (char c : fullMovesFen) {
        if (isdigit(c)) {
            board->fullMoves *= 10;
            board->fullMoves += (c - '0');
        }
    }
}


void loadAllBoard(Board* board, std::string fen) {
    std::vector<std::string> separatedFen = separateFEN(fen);

    loadBoard(board, separatedFen[BOARD]);
    loadActiveTeam(board, separatedFen[ACTIVE_TEAM]);
    loadCastleRights(board, separatedFen[CASTLE_RIGHTS]);
    loadEnPassantSquare(board, separatedFen[EN_PASSANT_PLACE]);
    loadHalfMoves(board, separatedFen[HALF_MOVES]);
    loadFullMoves(board, separatedFen[FULL_MOVES]);
}
