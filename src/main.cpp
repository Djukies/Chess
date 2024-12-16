#include <iostream>
#include <raylib.h>
#include <vector>
#include <chrono>
#include <iomanip>

#include "Renderer/Renderer.h"
#include "Debug/Debug.h"
#include "Types.h"
#include "Board/Board.h"
#include "GameLogic/GameLogic.h"

Board createBoardDupe(Board* board) {
    return {board->whitePieces,
                       board->blackPieces,
                       board->pawns,
                       board->knights,
                       board->bishops,
                       board->rooks,
                       board->queens,
                       board->kings,
                       board->blackKingPos,
                       board->whiteKingPos,
                       board->castleRights,
                       board->enPassantSquare,
                       0, 0,
                       board->activeTeam};
}

bool isSame(Board* board, Board newBoard) {

    return (newBoard.whitePieces == board->whitePieces &&
            newBoard.blackPieces == board->blackPieces &&
            newBoard.pawns == board->pawns &&
            newBoard.knights == board->knights &&
            newBoard.bishops == board->bishops &&
            newBoard.rooks == board->rooks &&
            newBoard.queens == board->queens &&
            newBoard.kings == board->kings &&
            newBoard.blackKingPos == board->blackKingPos &&
            newBoard.whiteKingPos == board->whiteKingPos &&
            newBoard.castleRights == board->castleRights &&
            newBoard.enPassantSquare == board->enPassantSquare &&
            newBoard.activeTeam == board->activeTeam);
}

void findDifference(Board* board, Board newBoard) {
    if (newBoard.whitePieces != board->whitePieces)
        std::cout << "whitePieces";
    if (newBoard.blackPieces != board->blackPieces)
        std::cout << "blackPieces";
    if (newBoard.pawns != board->pawns)
        std::cout << "pawns";
    if (newBoard.knights != board->knights)
        std::cout << "knights";
    if (newBoard.bishops != board->bishops)
        std::cout << "bishops";
    if (newBoard.rooks != board->rooks)
        std::cout << "rooks";
    if (newBoard.queens != board->queens)
        std::cout << "queens";
    if (newBoard.kings != board->kings)
        std::cout << "kings";
    if (newBoard.blackKingPos != board->blackKingPos)
        std::cout << "blackKingPos";
    if (newBoard.whiteKingPos != board->whiteKingPos)
        std::cout << "whiteKingPos";
    if (newBoard.castleRights != board->castleRights)
        std::cout << "castleRights";
    if (newBoard.enPassantSquare != board->enPassantSquare)
        std::cout << "enPassantSquare";
    if (newBoard.activeTeam != board->activeTeam)
        std::cout << "activeTeam";
}

Piece getPieceAtPos(Board* board, integer pos) {
    bit_board mask = 1ULL << pos;
    if (mask & board->pawns) return PAWN;
    if (mask & board->bishops) return BISHOP;
    if (mask & board->knights) return KNIGHT;
    if (mask & board->rooks) return ROOK;
    if (mask & board->queens) return QUEEN;
    if (mask & board->kings) return KING;
    return NONE;
}

char xToChar(int x) {
    switch (x) {
        case (0):
            return 'a';
        case (1):
            return 'b';
        case (2):
            return 'c';
        case (3):
            return 'd';
        case (4):
            return 'e';
        case (5):
            return 'f';
        case (6):
            return 'g';
        case (7):
            return 'h';
        default:
            return 'x';
    }
}

double moveCount = 0;
int captures = 0;
int enPassants = 0;
int checkmates = 0;
void MoveGenerationTest(Board* board, const int depth, GameLogic* gameLogic/*, std::vector<Move> moveList*/)
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
            /*Board boardDupe = createBoardDupe(board);*/
            makeMove(board, move);
            /*moveList.push_back(move);*/
            MoveGenerationTest(board, depth - 1,  gameLogic/*, moveList*/);
            unmakeMove(board, move);
            /*if (!isSame(board, boardDupe)) {
                Piece piece = getPieceAtPos(board, move.oldPos);
                errors++;
                std::cout << "Error at: ";
                for (Move move: moveList) {
                    std::cout << xToChar(intToX(move.oldPos)) << intToY(move.oldPos) + 1;
                    std::cout << xToChar(intToX(move.newPos)) << intToY(move.newPos) + 1;
                    std::cout << " ";
                }
                std::cout << ". Error with the ";
                findDifference(board, boardDupe);
                std::cout << std::endl;

                board->whitePieces = boardDupe.whitePieces;
                board->blackPieces = boardDupe.blackPieces;
                board->pawns = boardDupe.pawns;
                board->knights = boardDupe.knights;
                board->bishops = boardDupe.bishops;
                board->rooks = boardDupe.rooks;
                board->queens = boardDupe.queens;
                board->kings = boardDupe.kings;
                board->blackKingPos = boardDupe.blackKingPos;
                board->whiteKingPos = boardDupe.whiteKingPos;
                board->castleRights = boardDupe.castleRights;
                board->enPassantSquare = boardDupe.enPassantSquare;
            }*/
            /*moveList.pop_back();*/
        }
    }
}


int main() {


    SetTargetFPS(60);
    SetTraceLogLevel(LOG_ERROR);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "Chess 2-Player by Djukies");

    SetWindowMinSize(300, 300);

    std::string FEN = "RNBQKBNR/PPPPPPPP/8/8/8/8/pppppppp/rnbqkbnr w KQkq - 0 10";//RNBQKBNR/PPPPPPPP/8/8/8/8/pppppppp/rnbqkbnr w KQkq - 0 10
    Board* board = loadBoardFromFEN(FEN);
    auto* gameLogic = new GameLogic(board);
    auto* renderer = new Renderer(board, gameLogic);
    auto* debug = new Debug(renderer, board);

    gameLogic->moves = gameLogic->calculateMoves();
    const int depth = 6;
    for (int checkDepth = 0; checkDepth <= depth; checkDepth++) {
        moveCount = 0;
        captures = 0;
        enPassants = 0;
        const auto start = std::chrono::high_resolution_clock::now();
        MoveGenerationTest(board, checkDepth, gameLogic);
        const auto end = std::chrono::high_resolution_clock::now();
        const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Depth: " << checkDepth << ", Moves: " << std::fixed << std::setprecision(0) << moveCount << ", Time taken: " << time.count() << "ms" << " (Captures: " << captures << ", En Passants: " << enPassants << ")" << std::endl;    }
    int i;
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_B)) {
            i++;
            i %= 8;
        }

        BeginDrawing();
        renderer->render();
        debug->debugAll();
        gameLogic->undoMoves();
        EndDrawing();
    } CloseWindow();

    return 0;
}
