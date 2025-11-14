#pragma once

#include <string>
#include <stdexcept>
#include <cstdio>
#include <map>
#include "../../Types.h"


PerftResult Stockfish(std::string fen, int depth);

std::string StockfishGetMove(std::vector<Move> madeMoves, std::string fen);

class StockfishEngine {
private:
    int moveTime = 1;

    int child_pid = 0;
    // File descriptors for the two pipes (read end, write end)
    int to_engine[2] = {-1, -1};   // Parent writes to [1], Engine reads from [0]
    int from_engine[2] = {-1, -1}; // Engine writes to [1], Parent reads from [0]
    
    // Convert file descriptor to FILE* for standard I/O
    FILE* engine_in = nullptr;
    FILE* engine_out = nullptr;

    void cleanup();
    std::string read_until(const std::string& terminator, bool includeTerminator = false);

public:
    StockfishEngine(const std::string& path);
    ~StockfishEngine();

    std::string read_raw_perft_output(const std::string &terminator);

    // The function you wanted to implement
    PerftResult runPerft(const std::string& fen, int depth);
    std::string runBestMove(std::string madeMoves, std::string fen);

    // Helper functions for UCI commands
    void send_command(const std::string& cmd);
};