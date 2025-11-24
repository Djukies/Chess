#include "Bot.h"

#include <algorithm>
#include <iostream>
#include <ostream>

#include "../GameLogic/MoveGeneration/MoveGeneration.h"

Bot::Bot() {
    std::cout << "ENGINE: " << "Creating Bot" << std::endl;
}

void Bot::setUpBoard(std::string FEN, std::vector<std::string> moves) {
    board = loadBoardFromFEN(FEN);
    std::cout << "ENGINE: " << "setting up: " << FEN;
    if (!moves.empty()) {
        std::cout << " moves: " << std::endl;
    }
    for (std::string move : moves) {
        std::cout << " " << move << std::endl;
    }
    std::cout<< std::endl;

    board->movesVector = calculateLegalMoves(board);
    board->movesMap = gamelogic->moveVectorToMap(board->movesVector);
    for (std::string move : moves) {

        std::cout << move << " " << gamelogic->tryMove(board, charToInt(move.substr(0, 2)), charToInt(move.substr(2, 2))) << std::endl;
    }

    algorithm->board = board;
}

void Bot::findBestMove(int depth) {
    std::cout << "ENGINE: " << "Searching for BestMove" << depth << std::endl;

    algorithm->stopFlag = false;
    // Kill old search thread if still running
    if (searchThread.joinable()) {
        algorithm->stopFlag = true;
        searchThread.join();
    }

    searchThread = std::thread([this, depth] {
        algorithm->calcBestMove(depth, board->whiteToMove);

        // Only print bestmove **after** search finishes

        this->sendToGUI("bestmove " + moveToString(algorithm->bestMove));
    });
}