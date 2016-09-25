

#include <iostream>

#include "../src/game-server/controller/Controller.hpp"

static Controller controller;

int main() {
    std::cout << controller.getText() << "\n";

    return 0;
}