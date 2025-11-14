#include "UCI.h"

#include <sstream>

void UCI::sendToEngine(const std::string& command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (cmd == "uci") {
        handleUCI();
    }
    else if (cmd == "isready") {
        handleIsReady();
    }
    else if (cmd == "setoption") {
        handleSetOption(iss);
    }
    else if (cmd == "ucinewgame") {
        handleNewGame();
    }
    else if (cmd == "position") {
        handlePosition(iss);
    }
    else if (cmd == "go") {
        handleGo(iss);
    }
    else if (cmd == "stop") {
        handleStop();
    }
    else if (cmd == "quit") {
        handleQuit();
    }
}

void UCI::handleUCI() {
    // Send engine identification
    std::cout << "id name DjukeUCI 1.0" << std::endl;
    std::cout << "id author Djukies" << std::endl;

    // Send available options
    /*std::cout << "option name Hash type spin default 128 min 1 max 16384" << std::endl;
    std::cout << "option name Threads type spin default 1 min 1 max 512" << std::endl;
    std::cout << "option name Ponder type check default false" << std::endl;*/

    std::cout << "uciok" << std::endl;

    initEngine();
}

void UCI::handleIsReady() {
    isReady();
    std::cout << "readyok" << std::endl;
}

void UCI::handleSetOption(std::istringstream& iss) {
    std::string token, name, value;

    // Parse "name <name> value <value>"
    iss >> token; // skip "name"

    // Get option name (may be multiple words)
    while (iss >> token && token != "value") {
        if (!name.empty()) name += " ";
        name += token;
    }

    // Get option value (may be multiple words)
    while (iss >> token) {
        if (!value.empty()) value += " ";
        value += token;
    }

    setOption(name, value);
}

void UCI::handleNewGame() {
    newGame();
}

void UCI::handlePosition(std::istringstream& iss) {
    std::string token, fen;
    std::vector<std::string> moves;

    iss >> token; // "startpos" or "fen"

    if (token == "startpos") {
        fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        iss >> token; // possibly "moves"
    }
    else if (token == "fen") {
        // Read FEN string (6 parts)
        for (int i = 0; i < 6 && iss >> token; i++) {
            if (!fen.empty()) fen += " ";
            fen += token;
        }
        iss >> token; // possibly "moves"
    }

    // Parse moves if present
    if (token == "moves") {
        while (iss >> token) {
            moves.push_back(token);
        }
    }

    setupPosition(fen, moves);
}

void UCI::handleGo(std::istringstream& iss) {
    std::string token;
    int depth = -1;
    int movetime = -1;
    int wtime = -1;
    int btime = -1;
    int winc = 0;
    int binc = 0;
    int movestogo = -1;
    bool infinite = false;

    while (iss >> token) {
        if (token == "depth") {
            iss >> depth;
        }
        else if (token == "movetime") {
            iss >> movetime;
        }
        else if (token == "wtime") {
            iss >> wtime;
        }
        else if (token == "btime") {
            iss >> btime;
        }
        else if (token == "winc") {
            iss >> winc;
        }
        else if (token == "binc") {
            iss >> binc;
        }
        else if (token == "movestogo") {
            iss >> movestogo;
        }
        else if (token == "infinite") {
            infinite = true;
        }
    }

    startSearch(depth, movetime, wtime, btime, winc, binc, movestogo, infinite);
}

void UCI::handleStop() {
    stopSearch();
}

void UCI::handleQuit() {
    quit();
}