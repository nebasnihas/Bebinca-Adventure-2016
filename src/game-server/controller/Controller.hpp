

#ifndef ADVENTURE2016_CONTROLLER_H
#define ADVENTURE2016_CONTROLLER_H

#include <string>
#include <map>
#include <functional>

#include "../../game-model/Command.hpp"
#include "../../game-model/GameModel.hpp"
#include "server.h"
#include "../messageSender.h"

using namespace networking;

class Controller {
public:
    Controller(GameModel &gameModel);
    void processCommand(const std::string& message, const Connection& connectionID, MessageSender& sender);
    void registerCommand(Command::Type type, string commandText, Command::functionRef method);

private:
    GameModel gameModel;

//    std::unordered_map<Connection, int> connectionToPlayerMap;
    std::unordered_map<int, Connection> playerToConnectionMap;

    std::map<std::string, Command> commandMap;
    string look(int playerID, vector<string> targets, MessageSender &messageSender);
    string executeCommand(const string &command, const vector<string> &targets, const Connection &connection,
                          MessageSender &messageSender);
    std::string Controller::authHandler(const std::string& args, const Connection& client, MessageSender& messageSender);

};


#endif //ADVENTURE2016_CONTROLLER_H
