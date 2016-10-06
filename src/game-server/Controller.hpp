#ifndef ADVENTURE2016_CONTROLLER_H
#define ADVENTURE2016_CONTROLLER_H

#include <string>
#include <map>
#include <functional>
#include <vector>

#include "Command.hpp"
#include "game/GameModel.hpp"
#include "networking/server.h"
#include "game/protocols/PlayerCommand.hpp"
#include "DisplayMessageBuilder.hpp"

using namespace networking;

class GameFunctions;

class Controller {
public:
    Controller(GameModel& gameModel, const std::vector<Connection>& allClients) : gameModel{gameModel}, allClients{allClients}{};
    DisplayMessageBuilder processCommand(const protocols::PlayerCommand& command, const Connection& client);
    void addNewPlayer(const PlayerInfo& player);
    void registerCommand(const Command& command);

    GameModel& getGameModel() const;
    const vector<Connection>& getAllClients() const;

private:
    std::unordered_map<Connection, std::string, ConnectionHash> clientToPlayerMap;
    std::unordered_map<std::string, Connection> playerToClientMap;
    std::unordered_map<std::string, Command>  playerCommandMap;

    GameModel& gameModel;
    const std::vector<Connection>& allClients;
};


#endif //ADVENTURE2016_CONTROLLER_H
