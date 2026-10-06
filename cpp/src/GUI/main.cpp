#include <iostream>
#include <raylib.h>
#include <vector>

#include "Renderer/Renderer.h"
#include "Debug/Debug.h"
#include "../Common/Types.h"
#include "../Common/Board/Board.h"
#include "GameLogic/GameLogic.h"
#include "../Common/PrecomputedData/PrecomputedData.h"
#include "../Common/MoveGeneration/MoveGeneration.h"
#include "../Engine/Algorithm/Algorithm.h"
#include "../Engine/Algorithm/Stockfish/Stockfish.h"
#include "../Engine/Algorithm/UCI/UCI.h"
#include "UCIGUI/UCIBridge.h"
#include "UCIGUI/UCIGUI.h"


int main() {

    // Set up the Window
    SetTargetFPS(60);
    SetTraceLogLevel(LOG_ERROR);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "Chess 2-Player by Djukies");
    SetWindowMinSize(300, 300);

    // Set up the Board
    std::string FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";//rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    Board* board = loadBoardFromFEN(FEN);
    board->algorithmWhite = true;
    board->algorithmBlack = true;
    // Set up the classes
    /*auto* algorithmWhite = new Algorithm(board);
    auto* algorithmBlack = new Algorithm(board);*/
    auto* gameLogic = new GameLogic(board);
    // auto* uci = new UCI();
    UCIProcess engine("./chess_engine");
    auto* ucigui = new UCIGUI();
    auto* renderer = new Renderer(board, gameLogic, /*uci*/nullptr);
    auto* debug = new Debug(renderer, board, gameLogic);
    renderer->FEN = FEN;
    debug->fen = FEN;
    gameLogic->algoWhiteStockfish = false;
    gameLogic->algoBlackStockfish = false;


    engine.send("uci");
    engine.send("isready");

    // Set the moves at beginning (rest will automatic after Move)
    precompute();
    board->movesVector = calculateLegalMoves(board);
    board->movesMap = moveVectorToMap(board->movesVector);


    // Test the moveGen (prints the amount of moves at every depth till depth:)
    const int depth = 0;

    std::cout << std::endl;
    debug->moveGenTest(depth);

    // Loop while the window shouldn't close
    while (!WindowShouldClose())
    {

        // Be able to undo Moves (only if key pressed)
        gameLogic->undoMoves(board);

        std::string line = engine.readLine();
        if (!line.empty()) {
            std::cout << "Engine sent: " << line;
            ucigui->checkForCommands(line);
        }

        if (ucigui->foundBestMove) {
            std::cout << "Found best move: " << ucigui->getBestMove() << std::endl;
            tryMove(board, charToInt(ucigui->bestMove.substr(0, 2)), charToInt(ucigui->bestMove.substr(2, 2)));
        }

        // All the rendering
        BeginDrawing();
            // In renderer checks for clicks etc
            renderer->render();
            // Debugs like showing bitboards
            debug->debugAll();
        EndDrawing();

    } CloseWindow();

    return 0;
}
