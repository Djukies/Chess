#ifndef UCIHANDLER_H
#define UCIHANDLER_H

#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <queue>


// Forward declarations of functions you'll implement
void initEngine();
void setOption(const std::string& name, const std::string& value);
void setupPosition(const std::string& fen, const std::vector<std::string>& moves);
void startSearch(int depth, int movetime, int wtime, int btime, int winc, int binc, int movestogo, bool infinite);
void stopSearch();
void quit();
void isReady();
void newGame();

// UCI command parser
class UCIBridge {
private:
    std::queue<std::string> engineOutput;
    std::function<void(const std::string&)> guiCallback;

public:
    // Set callback for engine output (GUI will receive messages here)
    void setGUICallback(std::function<void(const std::string&)> callback) {
        guiCallback = callback;
    }

    // GUI calls this to send command to engine
    void sendToEngine(const std::string& command);

    // Engine calls this to send message to GUI
    void sendToGUI(const std::string& message) {
        if (guiCallback) {
            guiCallback(message);
        } else {
            engineOutput.push(message);
        }
    }

    // GUI can poll for messages if not using callback
    std::string receiveFromEngine() {
        if (engineOutput.empty()) return "";
        std::string msg = engineOutput.front();
        engineOutput.pop();
        return msg;
    }

private:
    void handleUCI();
    void handleIsReady();
    void handleSetOption(std::istringstream &iss);
    void handleNewGame();
    void handlePosition(std::istringstream &iss);
    void handleGo(std::istringstream &iss);
    void handleStop();
    void handleQuit();
};

// Stub implementations - replace these with your actual engine code
inline void initEngine() {
    // Initialize your chess engine
}

inline void setOption(const std::string& name, const std::string& value) {
    // Set engine option (Hash, Threads, etc.)
    std::cerr << "Setting option: " << name << " = " << value << std::endl;
}

inline void setupPosition(const std::string& fen, const std::vector<std::string>& moves) {
    // Set up the board position from FEN and apply moves
    std::cerr << "Position FEN: " << fen << std::endl;
    std::cerr << "Moves: ";
    for (const auto& move : moves) {
        std::cerr << move << " ";
    }
    std::cerr << std::endl;
}

inline void startSearch(int depth, int movetime, int wtime, int btime, int winc, int binc, int movestogo, bool infinite) {
    // Start searching for best move
    std::cerr << "Starting search with depth=" << depth << ", movetime=" << movetime << std::endl;
    
    // Example output - replace with actual search
    // During search, you can output:
    // std::cout << "info depth 5 score cp 20 nodes 1000 time 500 pv e2e4 e7e5" << std::endl;
    
    // When done, output best move:
    std::cout << "bestmove e2e4" << std::endl;
}

inline void stopSearch() {
    // Stop the current search
    std::cerr << "Stopping search" << std::endl;
}

inline void isReady() {
    // Perform any necessary initialization to be ready
}

inline void newGame() {
    // Reset engine for new game
    std::cerr << "New game started" << std::endl;
}

inline void quit() {
    // Clean up and exit
    std::cerr << "Quitting engine" << std::endl;
}

#endif // UCIHANDLER_H
