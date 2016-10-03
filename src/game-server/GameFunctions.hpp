

#ifndef ADVENTURE2016_GAMEFUNCTIONS_H
#define ADVENTURE2016_GAMEFUNCTIONS_H


#include <game/GameModel.hpp>
#include "MessageSender.h"
#include "Command.hpp"

class GameFunctions {
public:
    void GameFunctions::look(const std::vector<std::string>& targets, const PlayerInfo& player, GameModel& gameModel, MessageSender& messageSender) {}
    void GameFunctions::move(const std::vector<std::string>& targets, const PlayerInfo& player, GameModel& gameModel, MessageSender& messageSender) {}
    void GameFunctions::listPlayers(const std::vector<std::string>& targets, const PlayerInfo& player, GameModel& gameModel, MessageSender& messageSender) {}
    void GameFunctions::listExits(const std::vector<std::string>& targets, const PlayerInfo& player, GameModel& gameModel, MessageSender& messageSender) {}
};


#endif //ADVENTURE2016_GAMEFUNCTIONS_H
