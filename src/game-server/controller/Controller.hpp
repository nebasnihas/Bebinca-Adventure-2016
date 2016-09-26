

#ifndef ADVENTURE2016_CONTROLLER_H
#define ADVENTURE2016_CONTROLLER_H

#include <string>
#include <map>
#include <functional>

#include "../../game-model/Command.hpp"
#include "../../game-model/GameModel.hpp"

class Controller {
public:
    Controller* getInstance();
    GameModel* getModel();

    void initController();

    std::string executeCommand(std::string command, std::vector<std::string> targets, int playerID);
    void registerCommand(Command::Type type, string commandText, Command::functionRef method);

private:
    Controller* instance;
    GameModel* gameModel;

    std::map<std::string, Command>* commandMap;
    string look(int playerID, vector<string> targets);

};


#endif //ADVENTURE2016_CONTROLLER_H
