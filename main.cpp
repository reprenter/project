#include "./include/GameManager.h"

#include <iostream>

int main() {
    try {
        GameManager game;
        game.run();
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
