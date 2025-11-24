#include "UCI.h"

#include <sstream>

#include "../../GameLogic/GameLogic.h"

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
    std::cout << "ENGINE: " << "id name DjukeUCI 1.0" << std::endl;
    std::cout << "ENGINE: " << "id author Djukies" << std::endl;
    sendToGUI("id name DjukeUCI 1.0");
    sendToGUI("id author Djukies");

    // Send available options
    /*std::cout << "ENGINE: " << "option name Hash type spin default 128 min 1 max 16384" << std::endl;
    std::cout << "ENGINE: " << "option name Threads type spin default 1 min 1 max 512" << std::endl;
    std::cout << "ENGINE: " << "option name Ponder type check default false" << std::endl;*/

    std::cout << "ENGINE: " << "uciok" << std::endl;
    sendToGUI("uciok");

    initEngine();
}

void UCI::handleIsReady() {
    isReady();
    std::cout << "ENGINE: " << "readyok" << std::endl;
    sendToGUI("readyok");
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


void UCI::initEngine() {
    bot = new Bot();
    bot->sendToGUI = [this](const std::string& msg) {
        this->sendToGUI(msg);    // your UCI's method
    };
}

void UCI::setOption(const std::string& name, const std::string& value) {
    // Set engine option (Hash, Threads, etc.)
    std::cerr << "ENGINE: " << "Setting option: " << name << " = " << value << std::endl;
}

void UCI::setupPosition(const std::string& fen, const std::vector<std::string>& moves) {
    // Set up the board position from FEN and apply moves
    std::cerr << "ENGINE: " << "Position FEN: " << fen << std::endl;
    std::cerr << "ENGINE: " << "Moves: ";
    for (const auto& move : moves) {
        std::cerr << "ENGINE: " << move << " ";
    }
    std::cerr << "ENGINE: " << std::endl;

    bot->setUpBoard(fen, moves);
}

void UCI::startSearch(int depth, int movetime, int wtime, int btime, int winc, int binc, int movestogo, bool infinite) {
    // Start searching for best move
    std::cerr << "ENGINE: " << "Starting search with depth=" << depth << ", movetime=" << movetime << std::endl;

    // Example output - replace with actual search
    // During search, you can output:
    // std::cout << "ENGINE: " << "info depth 5 score cp 20 nodes 1000 time 500 pv e2e4 e7e5" << std::endl;

    bot->findBestMove(depth);
}

void UCI::stopSearch() {
    // Stop the current search
    std::cerr << "ENGINE: " << "Stopping search" << std::endl;
}

void UCI::isReady() {
    // Perform any necessary initialization to be ready
}

void UCI::newGame() {
    // Reset engine for new game
    std::cerr << "ENGINE: " << "New game started" << std::endl;
}

void UCI::quit() {
    // Clean up and exit
    std::cerr << "ENGINE: " << "Quitting engine" << std::endl;
}