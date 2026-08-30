#include "UCIGUI.h"

#include <iostream>


void UCIGUI::checkForCommands(std::string command) {
    if (!command.empty()) {
        handleCommand(command);
    }
}


void UCIGUI::handleCommand(std::string &command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "id") {
        handleId(iss);
    }
    else if (cmd == "uciok") {
        handleUCIOk();
    }
    else if (cmd == "readyok") {
        handleReadyOk();
    }
    else if (cmd == "bestmove") {
        handleBestMove(iss);
    }
    else if (cmd == "info") {
        handleInfo(iss);
    }
    else if (cmd == "option") {
        handleOption(iss);
    }
}

void UCIGUI::handleId(std::istringstream &iss) {
    std::string token, fen, value;

    iss >> token;

    if (token == "name") {
        while (iss >> token) {
            if (!value.empty()) value += " ";
            value += token;
        }
        std::cerr << "GUI: name: " << value << std::endl;
    }
    else if (token == "author") {
        while (iss >> token) {
            if (!value.empty()) value += " ";
            value += token;
        }
        std::cerr << "GUI: author: " << value << std::endl;
    }

}

void UCIGUI::handleUCIOk() {
    std::cerr << "GUI: uciok" << std::endl;
}

void UCIGUI::handleReadyOk() {
    std::cerr << "GUI: readyok" << std::endl;
}

void UCIGUI::handleBestMove(std::istringstream &iss) {
    std::string move;
    iss >> move;
    std::cerr << "GUI: bestmove: " << move << std::endl;
    bestMove = move;
    foundBestMove = true;
}

void UCIGUI::handleInfo(std::istringstream &iss) {

}

void UCIGUI::handleOption(std::istringstream &iss) {

}

std::string UCIGUI::getBestMove() {
    if (!foundBestMove) {
        return "";
    }
    foundBestMove = false;
    return bestMove;
}

