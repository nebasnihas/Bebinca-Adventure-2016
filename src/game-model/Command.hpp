#ifndef ADVENTURE2016_COMMAND_H
#define ADVENTURE2016_COMMAND_H


#include <vector>
#include <string>
#include <functional>
#include "../game-server/controller/MessageSender.h"
#include "GameModel.hpp"

using namespace std;

class Command {
public:
    typedef function<void (const vector<string>& targets, int playerID, const Connection& clientID, GameModel& gameModel, MessageSender& messageSender)> functionRef;

    enum Type {
        AREA,
        CHARACTER,
        ENTITY,
        GAME
    };

    /**
     * Type supported by game controller for calling appropriate game functions.
     * Final parameter is a function pointer to the method that provides the functionality.
     * @param type
     * @param text
     * @param method
     * @return
     */
    Command(Type type, string text, functionRef method);
    std::string getText();
    functionRef getMethod();

private:
    Type type;
    std::string text;
    functionRef method;
};


#endif //ADVENTURE2016_COMMAND_H
