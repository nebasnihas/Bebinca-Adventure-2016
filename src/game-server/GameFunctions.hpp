

#ifndef ADVENTURE2016_GAMEFUNCTIONS_H
#define ADVENTURE2016_GAMEFUNCTIONS_H


#include <game/GameModel.hpp>
#include "Command.hpp"
#include "Controller.hpp"

using namespace std;
using namespace std::placeholders;


class GameFunctions {
public:
    GameFunctions(Controller& controller);

    DisplayMessageBuilder look(const vector<string> &targets, const PlayerInfo &player);
    DisplayMessageBuilder move(const std::vector<std::string> &targets, const PlayerInfo &player);
    DisplayMessageBuilder listPlayers(const std::vector<std::string> &targets, const PlayerInfo &player);
    DisplayMessageBuilder listExits(const std::vector<std::string> &targets, const PlayerInfo &player);
    DisplayMessageBuilder say(const std::vector<std::string> &targets, const PlayerInfo &player);

private:
    string getPlayerAreaID(const PlayerInfo &player);
    Controller& controller;
    GameModel& gameModel;
    const std::vector<Connection>& allClients;

};


#endif //ADVENTURE2016_GAMEFUNCTIONS_H
