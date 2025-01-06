#include "Debug.h"

Debug::Debug(Renderer* renderer, Board* board, GameLogic* gameLogic) : renderer(renderer), board(board), gameLogic(gameLogic) {

}

void Debug::debugAll() {
    if (IsKeyDown(KEY_LEFT_SHIFT)) {
        this->showBitBoards();
    }
    if (IsKeyPressed(KEY_L)) {
        currentSquare++;
    } else if (IsKeyPressed(KEY_K)) {
        currentSquare--;
    }
    if (IsKeyPressed(KEY_J)) {
        currentDir++;
        currentDir %= 8;
    }
}

void Debug::showBitBoards() {
    this->bitBoards = {0ULL,
                       board->pinnedPieces,
                       rays[board->friendlyKingPos][3],
                       board->opponentControlledSquares,
                       board->whitePieces|board->blackPieces, board->whitePieces, board->blackPieces, board->pawns, board->knights, board->bishops, board->rooks, board->queens, board->kings};
    if (IsKeyPressed(this->showNextBitboard)) this->currentBitBoard++;
    if (IsKeyPressed(this->showPreviousBitBoard)) this->currentBitBoard--;

    int bitBoardIndex = this->currentBitBoard % (int)this->bitBoards.size();
    if (bitBoardIndex == 0) return;
    renderer->drawBitBoard(this->bitBoards[bitBoardIndex]);
}

Board createBoardDupe(Board* board) {
    Board boardDupe;
    boardDupe.whitePieces = board->whitePieces;
    boardDupe.blackPieces = board->blackPieces;
    boardDupe.pawns = board->pawns;
    boardDupe.knights = board->knights;
    boardDupe.bishops = board->bishops;
    boardDupe.rooks = board->rooks;
    boardDupe.queens = board->queens;
    boardDupe.kings = board->kings;
    boardDupe.whiteToMove = board->whiteToMove;
    boardDupe.whiteKingPos = board->whiteKingPos;
    boardDupe.blackKingPos = board->blackKingPos;
    return boardDupe;
}

bool isSame(Board boardDupe, Board* board) {
    return boardDupe.whitePieces == board->whitePieces &&
    boardDupe.blackPieces == board->blackPieces &&
    boardDupe.pawns == board->pawns &&
    boardDupe.knights == board->knights &&
    boardDupe.bishops == board->bishops &&
    boardDupe.rooks == board->rooks &&
    boardDupe.queens == board->queens &&
    boardDupe.kings == board->kings &&
    boardDupe.whiteToMove == board->whiteToMove &&
    boardDupe.whiteKingPos == board->whiteKingPos &&
    boardDupe.blackKingPos == board->blackKingPos;
}

void findDifference(Board boardDupe, Board* board) {
    if (boardDupe.whitePieces != board->whitePieces) std::cout << "whitePieces" << board->whitePieces << " " << boardDupe.whitePieces;
    if (boardDupe.blackPieces != board->blackPieces) std::cout << "blackPieces" << board->blackPieces << " " << boardDupe.blackPieces;
    if (boardDupe.pawns != board->pawns) std::cout << "pawns" << board->pawns << " " << boardDupe.pawns;
    if (boardDupe.knights != board->knights) std::cout << "knights" << board->knights << " " << boardDupe.knights;
    if (boardDupe.bishops != board->bishops) std::cout << "bishops" << board->bishops << " " << boardDupe.bishops;
    if (boardDupe.rooks != board->rooks) std::cout << "rooks" << board->rooks << " " << boardDupe.rooks;
    if (boardDupe.queens != board->queens) std::cout << "queens" << board->queens << " " << boardDupe.queens;
    if (boardDupe.kings != board->kings) std::cout << "kings" << board->kings << " " << boardDupe.kings;
    if (boardDupe.whiteToMove != board->whiteToMove) std::cout << "whiteToMove" << board->whiteToMove << " " << boardDupe.whiteToMove;
    if (boardDupe.whiteKingPos != board->whiteKingPos) std::cout << "whiteKingPos" << (int)board->whiteKingPos << " " << (int)boardDupe.whiteKingPos;
    if (boardDupe.blackKingPos != board->blackKingPos) std::cout << "blackKingPos" << (int)board->blackKingPos << " " << (int)boardDupe.blackKingPos;
}

char intToChar(int x) {
    switch (x) {
        case 0:
            return 'a';
        case 1:
            return 'b';
        case 2:
            return 'c';
        case 3:
            return 'd';
        case 4:
            return 'e';
        case 5:
            return 'f';
        case 6:
            return 'g';
        case 7:
            return 'h';
        default:
            return 'z';
    }
}

double moveCount = 0;
int captures = 0;
int enPassants = 0;
void MoveGenerationTest(Board* board, const int depth)
{

    if (depth == 0)
    {
        return;
    }
    std::vector<Move> moves = calculateLegalMoves(board);

    if (depth == 1) {
        moveCount += (double) moves.size();
        for (Move move : moves) {
            if (containsSquare(board->whitePieces | board->blackPieces, (move & targetSquareMask) >> 6)) {captures++;}
            if (((move & flagMask) >> 12) == EnPassantCaptureFlag) {

                captures++;
                enPassants++;
            }
        }
        return;
    }
    for (const Move move : moves)
    {
        //moveList.push_back(Move);
        /*Board boardDupe = createBoardDupe(board);
        if (!isSame(boardDupe, board)) {
            std::cout << "Is different after INITALIZING";
            findDifference(boardDupe, board);
            std::cout << std::endl;
        }*/
        makeMove(board, move);
        MoveGenerationTest(board, depth - 1);
        unMakeMove(board, move);
        /*if (!isSame(boardDupe, board)) {
            if (true) {
                std::cout << "Is different after MAKE/UNMAKE";
                for (Move moveInList: moveList) {
                    int oldPos = moveInList & startSquareMask;
                    int newPos = (moveInList & targetSquareMask) >> 6;
                    std::cout << intToChar(intToX(oldPos)) << intToY(oldPos) + 1 << intToChar(intToX(newPos)) << intToY(newPos) + 1 << " ";
                }
                findDifference(boardDupe, board);
                std::cout << std::endl;
            }
        }
        moveList.pop_back();*/
    }
}

void Debug::moveGenTest(int depth) {
    for (int checkDepth = 0; checkDepth <= depth; checkDepth++) {
        moveCount = 0;
        captures = 0;
        enPassants = 0;
        const auto start = std::chrono::high_resolution_clock::now();
        MoveGenerationTest(board, checkDepth);
        const auto end = std::chrono::high_resolution_clock::now();
        const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Depth: " << checkDepth << ", Moves: " << std::fixed << std::setprecision(0) << moveCount << ", Time taken: " << time.count() << "ms" << " (Captures: " << captures << ", En Passants: " << enPassants << ")" << std::endl;    }
}
