

#ifndef ADVENTURE2016_CONTROLLER_H
#define ADVENTURE2016_CONTROLLER_H

#include <string>
#include <map>
#include <functional>

#include "Command.hpp"
#include "game/GameModel.hpp"
#include "networking/server.h"
#include "MessageSender.h"

using namespace networking;

class Controller {
public:
    Controller();
    void processCommand(const std::string& message, const Connection& client, GameModel& gameModel, MessageSender& messageSender);
    void registerCommand(const Command::Type& type, const std::string& commandText, Command::functionRef method);

private:
    std::unordered_map<Connection, int, ConnectionHash> clientToPlayerMap;
    std::unordered_map<int, Connection> playerToClientMap;

    std::map<std::string, Command> commandMap;

    void executeCommand(const std::string& command, const std::vector<std::string>& targets,
                        const Connection& client, GameModel& gameModel, MessageSender& messageSender);

    void look(const std::vector<std::string>& targets, int playerID, const Connection& clientID, GameModel& gameModel, MessageSender& messageSender);
    void authHandler(const std::vector<std::string>& targets, int playerID, const Connection& clientID, GameModel& gameModel, MessageSender& messageSender);
    void sayHandler(const std::vector<std::string>& targets, int playerID, const Connection& clientID, GameModel& gameModel, MessageSender& messageSender);
};


#endif //ADVENTURE2016_CONTROLLER_H
