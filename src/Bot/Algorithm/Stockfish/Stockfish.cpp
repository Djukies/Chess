#include "Stockfish.h" // Assuming this is where the class definition is located
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <algorithm>
#include <sstream>

// --- Utility Functions (Private) ---

void StockfishEngine::cleanup() {
    if (child_pid > 0) {
        // Close the I/O streams
        if (engine_in) fclose(engine_in);
        if (engine_out) fclose(engine_out);

        // Terminate the child process and wait for it
        kill(child_pid, SIGTERM);
        waitpid(child_pid, NULL, 0);
    }

    // Close all file descriptors (safe to call even if they are -1)
    if (to_engine[0] != -1) close(to_engine[0]);
    if (to_engine[1] != -1) close(to_engine[1]);
    if (from_engine[0] != -1) close(from_engine[0]);
    if (from_engine[1] != -1) close(from_engine[1]);
}

std::string StockfishEngine::read_until(const std::string& terminator, bool includeTerminator) {
    if (!engine_out) return "";

    std::string output;
    char buffer[256];

    // Set engine_out to be unbuffered to get real-time output
    setvbuf(engine_out, NULL, _IONBF, 0);

    while (fgets(buffer, sizeof(buffer), engine_out) != NULL) {
        std::string line = buffer;
        output += line;

        // Debugging print
        //std::cerr << "Engine RCV: " << line;

        if (line.find(terminator) != std::string::npos) {
            if (!includeTerminator) {
                // If we don't include the terminator, remove the last line
                size_t pos = output.find(terminator);
                if (pos != std::string::npos) {
                    output.resize(pos + terminator.length());
                    // Find the start of the line containing the terminator
                    size_t last_newline = output.rfind('\n', pos);
                    if (last_newline != std::string::npos) {
                        output.resize(last_newline);
                    } else {
                        output.clear(); // If no newline, just clear the whole thing.
                    }
                }
            }
            break;
        }
    }
    return output;
}

void StockfishEngine::send_command(const std::string& cmd) {
    if (engine_in) {
        fprintf(engine_in, "%s\n", cmd.c_str());
        fflush(engine_in);
        //std::cerr << "Engine SND: " << cmd << std::endl; // Debugging print
    }
}

// --- Constructor / Destructor (Public) ---

StockfishEngine::StockfishEngine(const std::string& path) {
    // 1. Create two pipes
    if (pipe(to_engine) == -1 || pipe(from_engine) == -1) {
        throw std::runtime_error("Failed to create pipes.");
    }

    // 2. Fork the process
    child_pid = fork();

    if (child_pid < 0) {
        cleanup();
        throw std::runtime_error("Failed to fork process.");
    }

    if (child_pid == 0) {
        // --- CHILD PROCESS (Stockfish) ---

        // Map the engine's STDIN to the read end of 'to_engine'
        dup2(to_engine[0], STDIN_FILENO);

        // Map the engine's STDOUT/STDERR to the write end of 'from_engine'
        dup2(from_engine[1], STDOUT_FILENO);
        dup2(from_engine[1], STDERR_FILENO);

        // Close all unused file descriptors in the child
        close(to_engine[0]); close(to_engine[1]);
        close(from_engine[0]); close(from_engine[1]);

        // Execute Stockfish (execl expects const char* arguments)
        if (execl(path.c_str(), path.c_str(), (char*)NULL) == -1) {
            // If execl fails, print an error and exit the child process
            perror("execl failed");
            _exit(1);
        }
    } else {
        // --- PARENT PROCESS (Your chess game) ---

        // Close the unused ends of the pipes
        close(to_engine[0]); // Parent won't read from to_engine
        close(from_engine[1]); // Parent won't write to from_engine

        // Convert the file descriptors to FILE* streams
        // Parent writes to engine's stdin (to_engine[1])
        engine_in = fdopen(to_engine[1], "w");
        // Parent reads from engine's stdout (from_engine[0])
        engine_out = fdopen(from_engine[0], "r");

        if (!engine_in || !engine_out) {
            cleanup();
            throw std::runtime_error("Failed to fdopen pipe ends.");
        }

        //read_until("Stockfish", true);

        // Initialize UCI
        send_command("uci");
        send_command("isready");
        read_until("readyok", true); // Wait for the engine to be ready
    }
}

StockfishEngine::~StockfishEngine() {
    send_command("quit");
    cleanup();
}

// --- Public API ---

std::string StockfishEngine::read_raw_perft_output(const std::string& terminator) {
    if (!engine_out) return "";

    std::string output;
    char buffer[256];

    // Set engine_out to be unbuffered to get real-time output
    setvbuf(engine_out, NULL, _IONBF, 0);

    while (fgets(buffer, sizeof(buffer), engine_out) != NULL) {
        std::string line = buffer;
        output += line;

        // Find the "Nodes searched" terminator
        if (line.find(terminator) != std::string::npos) {
            break;
        }
    }
    return output;
}

PerftResult StockfishEngine::runPerft(const std::string& fen, int depth) {
    PerftResult result;

    send_command("position fen " + fen);
    send_command("go perft " + std::to_string(depth));

    // 1. Read the raw output (including the final total line)
    std::string raw_output = read_raw_perft_output("Nodes searched:");
    result.full_breakdown = raw_output; // Store the raw text if needed

    // Use a string stream to easily process the output line by line
    std::stringstream ss(raw_output);
    std::string line;

    while (std::getline(ss, line)) {
        // Find the position of the colon ':'
        size_t colon_pos = line.find(':');

        if (colon_pos != std::string::npos) {
            // This is a move breakdown line (e.g., "a2a3: 2186")

            std::string move_str = line.substr(0, colon_pos);
            std::string nodes_str = line.substr(colon_pos + 1);

            // Trim whitespace from move and nodes strings
            move_str.erase(0, move_str.find_first_not_of(" \t\n\r"));
            move_str.erase(move_str.find_last_not_of(" \t\n\r") + 1);
            nodes_str.erase(0, nodes_str.find_first_not_of(" \t\n\r"));
            nodes_str.erase(nodes_str.find_last_not_of(" \t\n\r") + 1);

            // Attempt to convert the nodes string to an integer
            try {
                int nodes = std::stoi(nodes_str);
                result.move_nodes[move_str] = nodes;
            } catch (const std::exception& e) {
                // Ignore lines that don't successfully convert (like "Nodes searched: 8902")
                // We handle the total separately for safety
            }
        }

        // 2. Check for the final total node count
        size_t total_pos = line.find("Nodes searched: ");
        if (total_pos != std::string::npos) {
            std::string total_str = line.substr(total_pos + 16); // +16 to skip "Nodes searched: "
            total_str.erase(0, total_str.find_first_not_of(" \t\n\r"));
            total_str.erase(total_str.find_last_not_of(" \t\n\r") + 1);

            try {
                result.total_nodes = std::stoll(total_str); // Use stoll for potentially large numbers
            } catch (const std::exception& e) {
                // If parsing the total fails, it remains 0.
            }
        }
    }

    return result;
}


PerftResult Stockfish(std::string fen, int depth) {

    const std::string STOCKFISH_PATH = "/usr/games/stockfish";

    try {
        StockfishEngine engine(STOCKFISH_PATH);
        //std::cout << "Stockfish is ready." << std::endl;

        // 1. Run the perft command and get the structured result
        PerftResult result = engine.runPerft(fen, depth);

        // 2. Print the total nodes
        //std::cout << "--- Perft Result ---\n";
        //std::cout << "Total Nodes Searched: " << result.total_nodes << std::endl;


        // 3. Print the dictionary contents (move breakdown)
        /*std::cout << "\nMove Breakdown (Map Contents):\n";
        for (const auto& pair : result.move_nodes) {
            std::cout << "  Move: " << pair.first << ", Nodes: " << pair.second << "\n";
        }*/
        result.move_nodes.erase("Nodes searched");
        return result;

    } catch (const std::exception& e) {
        std::cerr << "\nFATAL Error: " << e.what() << std::endl;
    }

    return {};
}

std::string StockfishEngine::runBestMove(std::string madeMoves, std::string fen) {
    PerftResult result;

    send_command("position fen " + fen + " moves" + madeMoves);
    send_command("go movetime " + std::to_string(moveTime));

    // 1. Read the raw output (including the final total line)
    std::string raw_output = read_until("bestmove", true);
    result.full_breakdown = raw_output; // Store the raw text if needed

    // Use a string stream to easily process the output line by line
    std::stringstream ss(raw_output);
    std::string line;

    while (std::getline(ss, line)) {
        size_t total_pos = line.find("bestmove ");
        if (total_pos != std::string::npos) {
            // Start after "bestmove " (9 characters)
            std::string full_move_line = line.substr(total_pos + 9);

            // Find the first space, which separates the move from "ponder"
            size_t space_pos = full_move_line.find(' ');

            std::string best_move;

            if (space_pos != std::string::npos) {
                // If a space is found (i.e., there is a ponder move), take only the part before it
                best_move = full_move_line.substr(0, space_pos);
            } else {
                // No space found, this is just the move to the end of the line
                best_move = full_move_line;
            }

            // Trim any trailing whitespace/newlines that might remain
            best_move.erase(best_move.find_last_not_of(" \t\n\r") + 1);

            // You should now store 'best_move' somewhere, not 'total_str'
            std::cout << "Best Move (finished): " << best_move << std::endl;

            // Since 'bestmove' is the final output, you likely want to break the loop here.
            // Also, if you want to return the move, you should assign it to a variable
            // that is accessible outside the loop.
            // For example, if 'result' is accessible: result.best_move = best_move;

            return best_move; // Exit the loop/function immediately after finding the move
        }
    }

    return " ";
}

std::string StockfishGetMove(std::vector<Move> madeMoves, std::string fen) {
    const std::string STOCKFISH_PATH = "/usr/games/stockfish";

    try {
        StockfishEngine engine(STOCKFISH_PATH);
        //std::cout << "Stockfish is ready." << std::endl;

        // 1. Run the perft command and get the structured result
        std::string madeMovesString = "";
        for (Move move : madeMoves) {
            //std::cout << (int)(move & startSquareMask) << " " << (int)((move & targetSquareMask) >> 6) << std::endl;
            std::string moveStr = intToString(move & startSquareMask) + intToString((move & targetSquareMask) >> 6);
            madeMovesString += " " + moveStr;
        }
        std::cout << madeMovesString << std::endl;
        std::string result = engine.runBestMove(madeMovesString, fen);

        // 2. Print the total nodes
        //std::cout << "--- Perft Result ---\n";
        //std::cout << "Total Nodes Searched: " << result.total_nodes << std::endl;


        // 3. Print the dictionary contents (move breakdown)
        /*std::cout << "\nMove Breakdown (Map Contents):\n";
        for (const auto& pair : result.move_nodes) {
            std::cout << "  Move: " << pair.first << ", Nodes: " << pair.second << "\n";
        }*/
        return result;

    } catch (const std::exception& e) {
        std::cerr << "\nFATAL Error: " << e.what() << std::endl;
    }

    return "";
}