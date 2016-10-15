

#ifndef ADVENTURE2016_GAMEFUNCTIONS_H
#define ADVENTURE2016_GAMEFUNCTIONS_H


#include <game/GameModel.hpp>
#include "Command.hpp"
#include "Controller.hpp"


class GameFunctions {
public:
    GameFunctions(Controller& controller);

    DisplayMessageBuilder look(const std::vector<std::string> &targets, const PlayerInfo &player);
    DisplayMessageBuilder move(const std::vector<std::string> &targets, const PlayerInfo &player);
    DisplayMessageBuilder listPlayers(const std::vector<std::string> &targets, const PlayerInfo &player);
    DisplayMessageBuilder listExits(const std::vector<std::string> &targets, const PlayerInfo &player);
    DisplayMessageBuilder say(const std::vector<std::string> &targets, const PlayerInfo &player);

private:
    std::string getPlayerAreaID(const PlayerInfo &player);
    Controller& controller;
    GameModel& gameModel;
    const std::vector<Connection>& allClients;

};


#endif //ADVENTURE2016_GAMEFUNCTIONS_H
