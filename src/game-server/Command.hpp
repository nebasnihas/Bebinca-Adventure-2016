#ifndef ADVENTURE2016_COMMAND_H
#define ADVENTURE2016_COMMAND_H


#include <vector>
#include <string>
#include <functional>
#include "MessageSender.h"
#include "game/GameModel.hpp"

using namespace std;

struct PlayerInfo {
    int playerID;
    Connection clientID;
};

class Command {
public:
    typedef function<void (const vector<string>& targets, const PlayerInfo& player, GameModel& gameModel, MessageSender& messageSender)> functionRef;


    /**
     * Type supported by game controller for calling appropriate game functions.
     * Final parameter is a function pointer to the method that provides the functionality.
     * @param type
     * @param keyword
     * @param method
     * @return
     */
    Command(const string& keyword, functionRef method) : text{keyword}, method{method} {};
    std::string getKeyword() const;
    functionRef getMethod() const;

private:
    std::string text;
    functionRef method;
};


#endif //ADVENTURE2016_COMMAND_H
