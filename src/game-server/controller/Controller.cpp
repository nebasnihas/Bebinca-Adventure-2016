#include <clocale>
#include <vector>
#include "Controller.hpp"

using namespace std::placeholders;

Controller* Controller::getInstance() {
    if (instance == NULL) {
        instance = new Controller();
    }
    return instance;
}

std::string Controller::executeCommand(std::string command, std::vector<std::string> targets, int playerID) {
    Command::functionRef targetMethod;
    try {
        targetMethod = commandMap->at(command).getMethod();
        return targetMethod(playerID, targets);
    }
    catch (const std::out_of_range& oor) {
        return "Command not found";
    }
}

void Controller::initController() {
    this->gameModel = new GameModel();
    commandMap = new std::map<std::string, Command>;

    auto target = &Controller::look;
    Command::functionRef callLook = std::bind(target, this, _1, _2);
    registerCommand(Command::Type::AREA, string("look"), callLook);
}

void Controller::registerCommand(Command::Type type, std::string commandText, Command::functionRef method) {
    Command command{type, commandText, method};
    commandMap->insert(std::make_pair(commandText, command));
}

std::string Controller::look(int playerID, vector<string> targets) {
    auto character = gameModel->getCharacterByID(playerID);
    std::string areaDescription = gameModel->getAreaDescription(character->getAreaID());
    return areaDescription;
}

GameModel* Controller::getModel() {
    return gameModel;
}



