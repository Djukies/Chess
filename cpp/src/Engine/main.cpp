#include <iostream>
#include "Algorithm/UCI/UCI.h"

int main() {
    UCI* uci = new UCI();
    std::string line;

    while (std::getline(std::cin, line)) {
        uci->sendToEngine(line);
    }
    return 0;
}