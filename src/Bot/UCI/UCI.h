#ifndef CHESS_UCI_H
#define CHESS_UCI_H
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include "../Bot.h"



class UCI {
private:
    std::queue<std::string> engineOutput;
    Bot* bot = nullptr;
public:
    // GUI calls this to send command to engine
    void sendToEngine(const std::string& command);

    // Engine calls this to send message to GUI
    void sendToGUI(const std::string& message) {
        engineOutput.push(message);
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

    void initEngine();
    void setOption(const std::string& name, const std::string& value);
    void setupPosition(const std::string& fen, const std::vector<std::string>& moves);
    void startSearch(int depth, int movetime, int wtime, int btime, int winc, int binc, int movestogo, bool infinite);
    void stopSearch();
    void isReady();
    void newGame();
    void quit();
};


#endif //CHESS_UCI_H