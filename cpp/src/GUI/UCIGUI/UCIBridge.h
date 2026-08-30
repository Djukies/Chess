#include <string>
#include <iostream>
#include <sys/wait.h>

class UCIProcess {
private:
    pid_t pid;
    int pipe_to_engine[2];    // [0] read, [1] write
    int pipe_from_engine[2];  // [0] read, [1] write

public:
    FILE* in;   // write to engine
    FILE* out;  // read from engine

    UCIProcess(const std::string& enginePath) {
        if (pipe(pipe_to_engine) == -1 || pipe(pipe_from_engine) == -1) {
            throw std::runtime_error("Failed to create pipes");
        }

        pid = fork();
        if (pid == -1) {
            throw std::runtime_error("Failed to fork");
        }

        if (pid == 0) {
            // Child process (engine)
            dup2(pipe_to_engine[0], STDIN_FILENO);
            dup2(pipe_from_engine[1], STDOUT_FILENO);

            close(pipe_to_engine[0]);
            close(pipe_to_engine[1]);
            close(pipe_from_engine[0]);
            close(pipe_from_engine[1]);

            execl(enginePath.c_str(), enginePath.c_str(), nullptr);
            exit(1); // execl failed
        } else {
            // Parent process (GUI)
            close(pipe_to_engine[0]);
            close(pipe_from_engine[1]);

            in = fdopen(pipe_to_engine[1], "w");
            out = fdopen(pipe_from_engine[0], "r");
            setvbuf(out, nullptr, _IONBF, 0);  // No buffering
            setvbuf(in, nullptr, _IONBF, 0);   // No buffering on input too

            if (!in || !out) {
                throw std::runtime_error("Failed to create FILE streams");
            }
        }
    }

    void send(const std::string& msg) {
        std::cout << "GUI: " << msg << std::endl;
        fprintf(in, "%s\n", msg.c_str());
        fflush(in);
    }

    bool hasData(int timeout_ms = 0) {
        fd_set readfds;
        FD_ZERO(&readfds);
        int fd = fileno(out);
        FD_SET(fd, &readfds);

        struct timeval tv;
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;

        int result = select(fd + 1, &readfds, nullptr, nullptr, &tv);
        return result > 0;
    }

    std::string readLine() {
        if (!hasData(0)) {
            return "";  // No data available
        }
        char buffer[4096];
        if (fgets(buffer, 4096, out) == nullptr)
            return "";
        return std::string(buffer);
    }

    ~UCIProcess() {
        if (in) fclose(in);
        if (out) fclose(out);
        if (pid > 0) {
            kill(pid, SIGTERM);
            waitpid(pid, nullptr, 0);
        }
    }
};
