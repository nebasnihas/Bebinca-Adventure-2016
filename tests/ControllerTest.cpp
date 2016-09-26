

#include <iostream>

#include "../src/game-server/controller/Controller.hpp"

void addMockAreas();

static Controller controller;
GameModel* gameModel;

using namespace std;

int main() {
    controller.initController();
    gameModel = controller.getModel();
    Character character("Test");
    gameModel->createCharacter(character);
    character.setAreaID(1);
    addMockAreas();

    vector<string> targets;
    std::cout << controller.executeCommand(string("look"), targets, 1) << "\n";

    return 0;
}


void addMockAreas()
{
    Entity tree = Entity("tree", "This is a tree.");
    Entity rock = Entity("rock", "This is a rock.");
    Entity test = Entity("test", "This is a test.");

    Area a1(1, "Area1", std::vector<int>{2, 3}, std::vector<Entity>{});
    gameModel->addArea(a1);


    Area a2(2, "Area2", std::vector<int>{1, 3}, std::vector<Entity>{tree, rock});
    gameModel->addArea(a2);

    Area a3(3, "Area3", std::vector<int>{2}, std::vector<Entity>{test});
    gameModel->addArea(a3);
}