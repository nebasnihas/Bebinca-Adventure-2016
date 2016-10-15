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
    Controller(GameModel& gameModel, Server& server) : gameModel{gameModel}, server{server}{};

    void registerCommand(const Command& command);
    DisplayMessageBuilder processCommand(const protocols::PlayerCommand& command, const networking::Connection& client);

    void addNewPlayer(const PlayerInfo& player);
    void removePlayer(const networking::Connection& clientID);
    void disconnectPlayer(const std::string& playerID);

    const Connection& getClientID(const std::string& playerID) const;
    const std::string& getPlayerID(const networking::Connection& clientID) const;
    const std::vector<networking::Connection>& getAllClients() const;
    GameModel& getGameModel() const;

private:
    using PlayerMap = boost::bimap<
            boost::bimaps::unordered_set_of<std::string>,
            boost::bimaps::unordered_set_of<networking::Connection, networking::ConnectionHash>
            >;
    using PlayerMapPair = PlayerMap::value_type;

    //bimap from playerID to clientID
    PlayerMap playerMap;
    //keep a list of all connected clients, since its useful when sending messages
    std::vector<Connection> allClients;

    std::unordered_map<std::string, Command>  playerCommandMap;

    GameModel& gameModel;
    networking::Server& server;
};


#endif //ADVENTURE2016_CONTROLLER_H
