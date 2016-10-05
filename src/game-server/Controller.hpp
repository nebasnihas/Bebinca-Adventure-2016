#ifndef ADVENTURE2016_CONTROLLER_H
#define ADVENTURE2016_CONTROLLER_H

#include <string>
#include <map>
#include <functional>

#include "Command.hpp"
#include "game/GameModel.hpp"
#include "networking/server.h"
#include "MessageSender.h"
#include "game/protocols/PlayerCommand.hpp"

using namespace networking;

class Controller {
public:
    Controller();
    void processCommand(const protocols::PlayerCommand& command, const Connection& client, GameModel& gameModel,
                        MessageSender& messageSender);
    void addNewPlayer(const PlayerInfo& player);
    void registerForPlayerCommand(const Command& command);
private:
    typedef std::unordered_map<std::string, Command> CommandMap;
    std::unordered_map<Connection, int, ConnectionHash> clientToPlayerMap;
    std::unordered_map<int, Connection> playerToClientMap;

    CommandMap playerCommandMap;

    void look(const std::vector<std::string>& targets, const PlayerInfo& player, GameModel& gameModel, MessageSender& messageSender);
    void sayHandler(const std::vector<std::string>& targets, const PlayerInfo& player, GameModel& gameModel, MessageSender& messageSender);
};


#endif //ADVENTURE2016_CONTROLLER_H
