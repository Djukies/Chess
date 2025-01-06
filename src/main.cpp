#include <iostream>
#include <raylib.h>
#include <vector>

#include "Renderer/Renderer.h"
#include "Debug/Debug.h"
#include "Types.h"
#include "Board/Board.h"
#include "GameLogic/GameLogic.h"
#include "GameLogic/Precomputed Data/PrecomputedData.h"
#include "GameLogic/MoveGeneration/MoveGeneration.h"

int main() {

    // Set up the Window
    SetTargetFPS(60);
    SetTraceLogLevel(LOG_ERROR);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 800, "Chess 2-Player by Djukies");
    SetWindowMinSize(300, 300);

    // Set up the Board
    std::string FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";//rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    Board* board = loadBoardFromFEN(FEN);
    // Set up the classes
    auto* gameLogic = new GameLogic(board);
    auto* renderer = new Renderer(board, gameLogic);
    auto* debug = new Debug(renderer, board, gameLogic);

    // Set the moves at beginning (rest will automatic after Move)
    precompute();
    board->movesVector = calculateLegalMoves(board);
    board->movesMap = gameLogic->moveVectorToMap(board->movesVector);


    // Test the moveGen (prints the amount of moves at every depth till depth:)
    const int depth = 4;

    std::cout << std::endl;
    debug->newMoveGenTest(depth);
    calculateLegalMoves(board);
    // Loop while the window shouldn't close
    while (!WindowShouldClose())
    {
        // All the game logic
        gameLogic->undoMoves();

        // All the rendering
        BeginDrawing();
            renderer->render();
            debug->debugAll();
        EndDrawing();

    } CloseWindow();

    return 0;
}
