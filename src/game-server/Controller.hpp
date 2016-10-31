#ifndef ADVENTURE2016_CONTROLLER_H
#define ADVENTURE2016_CONTROLLER_H

#include <string>
#include <map>
#include <functional>
#include <vector>
#include <memory>

#include "Command.hpp"
#include "game/GameModel.hpp"
#include "networking/server.h"
#include "game/protocols/PlayerCommand.hpp"
#include "MessageBuilder.hpp"
#include "boost/bimap/unordered_set_of.hpp"
#include "boost/bimap.hpp"
#include "CommandCreator.hpp"

class GameFunctions;

class Controller {
public:
    Controller(GameModel& gameModel, networking::Server& server, CommandCreator& commandCreator);

    void registerCommand(const Command& command);
    std::unique_ptr<MessageBuilder> processCommand(const protocols::PlayerCommand& command,
                                                   const networking::Connection& client);

    void addNewPlayer(const PlayerInfo& player);
    void removePlayer(const networking::Connection& clientID);
    void disconnectPlayer(const std::string& playerID);

    const networking::Connection& getClientID(const std::string& playerID) const;
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
    std::vector<networking::Connection> allClients;

    std::unordered_map<std::string, std::shared_ptr<Command>>  playerCommandMap;

    GameModel& gameModel;
    networking::Server& server;
    CommandCreator commandCreator;

    //help command
    std::unique_ptr<MessageBuilder> help(const std::vector<std::string>& targets, const PlayerInfo& player);
    std::unique_ptr<MessageBuilder> allCommandsHelp(const networking::Connection& clientID);
    std::string getCommandBindingsHelpMessage(const std::string command);
};


#endif //ADVENTURE2016_CONTROLLER_H
