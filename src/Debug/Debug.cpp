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
                       rays[currentSquare][currentDir],
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

/*void remove_similarities(std::vector<std::vector<small_move>>& vecs1, std::vector<std::vector<small_move>>& vecs2) {
    for (auto& v1 : vecs1) {
        for (auto& v2 : vecs2) {
            v1.erase(std::remove_if(v1.begin(), v1.end(), [&v2](small_move num) {
                auto it = std::find(v2.begin(), v2.end(), num);
                if (it != v2.end()) {
                    v2.erase(it); // Remove the element from v2
                    return true;  // Mark for removal in v1
                }
                return false;
            }), v1.end());
        }
    }

    // Clear any empty vectors in vecs2
    for (auto& v2 : vecs2) {
        if (v2.empty()) {
            v2.clear();
        }
    }
}*/

char intToChar(int x) {
    switch (x) {
        case 0:
            return 'a';
            break;
        case 1:
            return 'b';
            break;
        case 2:
            return 'c';
            break;
        case 3:
            return 'd';
            break;
        case 4:
            return 'e';
            break;
        case 5:
            return 'f';
            break;
        case 6:
            return 'g';
            break;
        case 7:
            return 'h';
            break;
        default:
            return 'z';
            break;
    }
}

double moveCount = 0;
int captures = 0;
int enPassants = 0;

void MoveGenerationTest(Board* board, const int depth, GameLogic* gameLogic)
{

    if (depth == 0)
    {
        return;
    }
    for (const std::map<integer, std::vector<Move>> moveVector = gameLogic->calculateMoves();
            const auto &[fst, moves] : moveVector)
    {
        if (depth == 1) moveCount += (double)moves.size();
        for (const Move move : moves)
        {
            if (depth == 1) {
                if ((move.move_type == CAPTURE || move.move_type == CAPTURE_PROMOTION || move.move_type == EN_PASSANT)) {
                    captures++;
                    if (move.move_type == EN_PASSANT) {
                        enPassants++;
                    }
                }
                continue;
            }
            makeMove(board, move);
            MoveGenerationTest(board, depth - 1,  gameLogic);
            unmakeMove(board, move);
        }
    }
}

void Debug::moveGenTest(int depth) {
    for (int checkDepth = 0; checkDepth <= depth; checkDepth++) {
        moveCount = 0;
        captures = 0;
        enPassants = 0;
        const auto start = std::chrono::high_resolution_clock::now();
        MoveGenerationTest(board, checkDepth, gameLogic);
        const auto end = std::chrono::high_resolution_clock::now();
        const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Depth: " << checkDepth << ", Moves: " << std::fixed << std::setprecision(0) << moveCount << ", Time taken: " << time.count() << "ms" << " (Captures: " << captures << ", En Passants: " << enPassants << ")" << std::endl;    }
}

std::vector<small_move> moveList{};
void NewMoveGenerationTest(Board* board, const int depth, Renderer* renderer)
{

    if (depth == 0)
    {
        return;
    }
    std::vector<small_move> moves = calculateLegalMoves(board);

    if (depth == 1) {
        moveCount += (double) moves.size();
        return;
    }
    for (const small_move move : moves)
    {
        moveList.push_back(move);
        Board boardDupe = createBoardDupe(board);
        if (!isSame(boardDupe, board)) {
            std::cout << "Is different after INITALIZING";
            findDifference(boardDupe, board);
            std::cout << std::endl;
        }
        makeSmallMove(board, move);

        /*if (moveList[0] == create_move(11, 19) && moveList[1] == create_move(48, 40)) {
            BeginDrawing();
            renderer->render();
            EndDrawing();
            std::cin.ignore();
        }*/
        NewMoveGenerationTest(board, depth - 1, renderer);
        unMakeSmallMove(board, move);
        if (!isSame(boardDupe, board)) {
            if (true) {
                std::cout << "Is different after MAKE/UNMAKE";
                for (small_move moveInList: moveList) {
                    int oldPos = moveInList & startSquareMask;
                    int newPos = (moveInList & targetSquareMask) >> 6;
                    std::cout << intToChar(intToX(oldPos)) << intToY(oldPos) + 1 << intToChar(intToX(newPos)) << intToY(newPos) + 1 << " ";
                }
                findDifference(boardDupe, board);
                std::cout << std::endl;
            }
        }
        moveList.pop_back();
    }
}

void Debug::newMoveGenTest(int depth) {
    for (int checkDepth = 0; checkDepth <= depth; checkDepth++) {
        moveCount = 0;
        captures = 0;
        enPassants = 0;
        const auto start = std::chrono::high_resolution_clock::now();
        NewMoveGenerationTest(board, checkDepth, renderer);
        const auto end = std::chrono::high_resolution_clock::now();
        const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Depth: " << checkDepth << ", Moves: " << std::fixed << std::setprecision(0) << moveCount << ", Time taken: " << time.count() << "ms" << " (Captures: " << captures << ", En Passants: " << enPassants << ")" << std::endl;    }

    /*std::vector<std::vector<small_move>> vec1 = allMoves1;
    std::vector<std::vector<small_move>> vec2 = allMoves2;

    *//*vec1.reserve(allMoves1.size());
    for (Move move : allMoves1) {
        vec1.push_back(move.oldPos | (move.newPos << 6));
    }
    vec2.reserve(allMoves2.size());
    for (small_move move : allMoves2) {
        vec2.push_back(move & ~flagMask);
    }*//*

    remove_similarities(vec1, vec2);

    std::cout << "vecs1: \n";
    for (const auto& v : vec1) {
        if (!v.empty()) {
            for (int num : v) std::cout << num << " ";
            std::cout << "\n";
        }
    }

    std::cout << "vecs2: \n";
    for (const auto& v : vec2) {
        if (!v.empty()) {
            for (int num : v) std::cout << num << " ";
            std::cout << "\n";
        }
    }*/
}
