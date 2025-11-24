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
        0b01100000ULL << (8*7),
        0b01110000ULL << (8*7),
        (0b00001110ULL << (8*7)),
        (0b00011100ULL << (8*7)),
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

char intToString(int x) {
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

long long moveCount = 0;
int moveCountPerRootMove = 0;
int captures = 0;
int enPassants = 0;
int maxDepth = 0;
void MoveGenerationTestRecursive(Board* board, const int depth)
{

    if (depth == 0)
    {
        return;
    }
    std::vector<Move> moves = calculateLegalMoves(board);

    if (depth == 1) {
        moveCount += (double) moves.size();
        moveCountPerRootMove += (double) moves.size();
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
        MoveGenerationTestRecursive(board, depth - 1);
        unMakeMove(board, move);
        /*if (depth == maxDepth) {
            int oldPos = move & startSquareMask;
            int newPos = (move & targetSquareMask) >> 6;
            std::cout << intToString(intToX(oldPos)) << 8-intToY(oldPos) << intToString(intToX(newPos)) << 8-intToY(newPos) << ": " << moveCountPerRootMove << std::endl;
            moveCountPerRootMove = 0;
        }*/
        /*if (!isSame(boardDupe, board)) {
            if (true) {
                std::cout << "Is different after MAKE/UNMAKE";
                for (Move moveInList: moveList) {
                    int oldPos = moveInList & startSquareMask;
                    int newPos = (moveInList & targetSquareMask) >> 6;
                    std::cout << intToString(intToX(oldPos)) << intToY(oldPos) + 1 << intToString(intToX(newPos)) << intToY(newPos) + 1 << " ";
                }
                findDifference(boardDupe, board);
                std::cout << std::endl;
            }
        }
        moveList.pop_back();*/
    }
}

PerftResult perftResult = {};

void MoveGenerationTest(Board* board, const int depth) {
    moveCount = 0;
    captures = 0;
    enPassants = 0;
    std::vector<Move> moves = calculateLegalMoves(board);
    for (const Move move : moves) {
        moveCountPerRootMove = 0;
        makeMove(board, move);
        MoveGenerationTestRecursive(board, depth - 1);
        unMakeMove(board, move);
        int oldPos = move & startSquareMask;
        int newPos = (move & targetSquareMask) >> 6;
        std::string key =
            std::string(1, intToString(intToX(oldPos))) +
            std::to_string(8 - intToY(oldPos)) +
            std::string(1, intToString(intToX(newPos))) +
            std::to_string(8 - intToY(newPos));
        if (moveCountPerRootMove == 0) moveCountPerRootMove++;
        perftResult.move_nodes[key] = moveCountPerRootMove;
    }
    perftResult.total_nodes = moveCount;
    /*std::cout << "\nMove Breakdown (Map Contents):\n";
        for (const auto& pair : perftResult.move_nodes) {
            std::cout << "  Move: " << pair.first << ", Nodes: " << pair.second << "\n";
        }*/
}

bool compare_perft_results(const std::map<std::string, int>& map_a, const std::map<std::string, int>& map_b) {
    bool identical = true;

    // 1. Check if the total number of moves (map size) is the same.
    if (map_a.size() != map_b.size()) {
        std::cout << "❌ **TOTAL SIZE MISMATCH!**\n";
        std::cout << "   Your Generator found " << map_a.size() << " moves.\n";
        std::cout << "   Stockfish found " << map_b.size() << " moves.\n";
        identical = false;
    }

    //std::cout << "\n--- Detailed Perft Comparison ---\n";

    // 2. Iterate through Map A (Your Generator) and check against Map B (Stockfish).
    for (const auto& pair_a : map_a) {
        const std::string& move = pair_a.first;
        int nodes_a = pair_a.second;

        auto it_b = map_b.find(move);

        if (it_b == map_b.end()) {
            // Case A: Key exists in A but not in B. (You generated a move Stockfish didn't)
            std::cout << "❌ **MISSING MOVE:** Your generator found move **" << move
                      << "** (Nodes: " << nodes_a << "), but Stockfish did not.\n";
            identical = false;
        } else {
            // Case B: Key exists in both. Check if the values match.
            int nodes_b = it_b->second;
            if (nodes_a != nodes_b) {
                std::cout << "⚠️ **NODE COUNT MISMATCH for move " << move << ".\n";
                std::cout << "   - Your Count: " << nodes_a << "\n";
                std::cout << "   - SF Count:   " << nodes_b << "\n";
                std::cout << "   - Difference: " << nodes_a - nodes_b << " nodes.\n";
                identical = false;
            }
        }
    }

    // 3. Check for keys that exist in B but not in A (Stockfish found a move you didn't).
    for (const auto& pair_b : map_b) {
        const std::string& move = pair_b.first;

        if (map_a.find(move) == map_a.end()) {
            std::cout << "❌ **EXTRA MOVE:** Stockfish found move **" << move
                      << "** (Nodes: " << pair_b.second << "), but your generator did not.\n";
            identical = false;
        }
    }

    //std::cout << "\n-----------------------------------\n";
    if (identical) {
        //std::cout << "🎉 **PERFT SUCCESS!** All moves and node counts match.\n";
        // std::cout << "\n-----------------------------------\n";
    } else {
        std::cout << "💔 **PERFT FAILURE!** Differences detected.\n";
        std::cout << "\n-----------------------------------\n";
    }

    return identical;
}

void Debug::moveGenTest(int depth) {
    maxDepth = depth;
    for (int checkDepth = 1; checkDepth <= depth; checkDepth++) {
        const auto start = std::chrono::high_resolution_clock::now();
        MoveGenerationTest(board, checkDepth);
        const auto end = std::chrono::high_resolution_clock::now();
        const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        PerftResult stockfishResult = Stockfish(fen, checkDepth);

        /*std::cout << "\nStockfish Result:\n";
        std::cout << "\nMove Breakdown (Map Contents):\n";
        for (const auto& pair : stockfishResult.move_nodes) {
            std::cout << "  Move: " << pair.first << ", Nodes: " << pair.second << std::endl;;
        }*/

        compare_perft_results(perftResult.move_nodes, stockfishResult.move_nodes);
        std::cout << "Depth: " << checkDepth << ", Moves: " << std::fixed << std::setprecision(0) << moveCount << ", Time taken: " << time.count() << "ms" << " (Captures: " << captures << ", En Passants: " << enPassants << "), (Stockfish amount: " << stockfishResult.total_nodes << ", Correct: " << ((stockfishResult.total_nodes == moveCount) ? "True" : "False") << ")" << std::endl;
    }

    std::cout << StockfishGetMove({}, fen) << std::endl;

}
