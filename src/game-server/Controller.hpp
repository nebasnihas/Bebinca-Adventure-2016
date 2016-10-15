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
#include "boost/bimap/unordered_set_of.hpp"
#include "boost/bimap.hpp"

class GameFunctions;

class Controller {
public:
    Controller(GameModel& gameModel, const std::vector<networking::Connection>& allClients) : gameModel{gameModel}, allClients{allClients}{};
    DisplayMessageBuilder processCommand(const protocols::PlayerCommand& command, const networking::Connection& client);
    void addNewPlayer(const PlayerInfo& player);
    void removePlayer(const networking::Connection& cliendID);
    void removePlayer(const std::string& playerID);
    void registerCommand(const Command& command);

    GameModel& getGameModel() const;
    const std::vector<networking::Connection>& getAllClients() const;

private:
    using PlayerMap = boost::bimap<
            boost::bimaps::unordered_set_of<std::string>,
            boost::bimaps::unordered_set_of<networking::Connection, networking::ConnectionHash>
            >;
    using PlayerMapPair = PlayerMap::value_type;

    PlayerMap playerMap;
    std::unordered_map<std::string, Command>  playerCommandMap;

    GameModel& gameModel;
    const std::vector<Connection>& allClients;
};


#endif //ADVENTURE2016_CONTROLLER_H
