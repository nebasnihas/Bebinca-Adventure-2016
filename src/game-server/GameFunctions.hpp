

#ifndef ADVENTURE2016_GAMEFUNCTIONS_H
#define ADVENTURE2016_GAMEFUNCTIONS_H


#include <game/GameModel.hpp>
#include <memory>
#include "Command.hpp"
#include "Controller.hpp"
#include "DisplayMessageBuilder.hpp"


class GameFunctions {
public:
    GameFunctions(Controller& controller);

    std::unique_ptr<MessageBuilder> look(const std::vector<std::string>& targets, const PlayerInfo& player);
    std::unique_ptr<MessageBuilder> move(const std::vector<std::string>& targets, const PlayerInfo& player);
    std::unique_ptr<MessageBuilder> listPlayers(const std::vector<std::string>& targets, const PlayerInfo& player);
    std::unique_ptr<MessageBuilder> listExits(const std::vector<std::string>& targets, const PlayerInfo& player);
    std::unique_ptr<MessageBuilder> say(const std::vector<std::string>& targets, const PlayerInfo& player);
    std::unique_ptr<MessageBuilder> whisper(const std::vector<std::string> &targets, const PlayerInfo &player);
    std::unique_ptr<MessageBuilder> shout(const std::vector<std::string> &targets, const PlayerInfo &player);

private:
    std::string getPlayerAreaID(const PlayerInfo &player);
    Controller& controller;
    GameModel& gameModel;
    const std::vector<networking::Connection>& allClients;

};


#endif //ADVENTURE2016_GAMEFUNCTIONS_H
