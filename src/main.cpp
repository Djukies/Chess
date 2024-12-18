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
    auto* debug = new Debug(renderer, board, gameLogic);

    gameLogic->moves = gameLogic->calculateMoves();

    debug->moveGenTest(5);

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
