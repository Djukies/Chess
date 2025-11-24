#ifndef CHESS_UCIGUI_H
#define CHESS_UCIGUI_H
#include <string>
#include <sstream>


class UCIGUI {
private:
    void handleCommand(std::string& command);

    void handleId(std::istringstream &iss);
    void handleUCIOk();
    void handleReadyOk();
    void handleBestMove(std::istringstream &iss);
    void handleInfo(std::istringstream &iss);
    void handleOption(std::istringstream &iss);


public:
    void checkForCommands(std::string command);
    bool foundBestMove = false;
    std::string bestMove = "";
    std::string getBestMove();
};


#endif //CHESS_UCIGUI_H