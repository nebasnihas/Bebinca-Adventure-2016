#ifndef ADVENTURE2016_CONTROLLER_H
#define ADVENTURE2016_CONTROLLER_H

#include <string>
#include <map>
#include <vector>
#include <boost/optional.hpp>
#include <memory>
#include <commands/CommandHandle.hpp>

#include "commands/Command.hpp"
#include "game/GameModel.hpp"
#include "networking/server.h"
#include "game/protocols/PlayerCommand.hpp"
#include "commands/MessageBuilder.hpp"
#include "boost/bimap/unordered_set_of.hpp"
#include "boost/bimap.hpp"
#include "commands/CommandConfig.hpp"

class Controller {
public:
    Controller(GameModel& gameModel, networking::Server& server, const CommandConfig& commandCreator);

    void registerCommand(const std::string& commandId, Command& command);
    void processCommand(const protocols::PlayerCommand& command,
                        const networking::Connection& client);

    void addNewPlayer(const PlayerInfo& player);
    void removePlayer(const networking::Connection& clientID);
    void disconnectPlayer(const std::string& playerID);

    boost::optional<networking::Connection> getClientID(const std::string& playerID) const;
    std::string getPlayerID(const networking::Connection& clientID) const;
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

    std::unordered_map<std::string, CommandHandle>  inputToCommandMap;

    GameModel& gameModel;
    networking::Server& server;
    CommandConfig commandConfig;

    //help command
    class HelpCommand;
    std::shared_ptr<HelpCommand> helpCommand;
};


#endif //ADVENTURE2016_CONTROLLER_H
