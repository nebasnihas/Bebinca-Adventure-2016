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

class Controller {
public:
    Controller();
    DisplayMessageBuilder processCommand(const protocols::PlayerCommand& command, const Connection& client, GameModel& gameModel,
                        std::vector<Connection>& allClients);
    void addNewPlayer(const PlayerInfo& player);
    void registerForPlayerCommand(const Command& command);
private:
    typedef std::unordered_map<std::string, Command> CommandMap;
    std::unordered_map<Connection, std::string, ConnectionHash> clientToPlayerMap;
    std::unordered_map<std::string, Connection> playerToClientMap;

    CommandMap playerCommandMap;

    DisplayMessageBuilder lookHandler(const vector<string>& arguments, const PlayerInfo& player, GameModel& gameModel, const std::vector<Connection>& allClients);
    DisplayMessageBuilder sayHandler(const vector<string>& arguments, const PlayerInfo& player, GameModel& gameModel, const std::vector<Connection>& allClients);
};


#endif //ADVENTURE2016_CONTROLLER_H
