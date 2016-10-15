#ifndef ADVENTURE2016_COMMAND_H
#define ADVENTURE2016_COMMAND_H


#include <vector>
#include <string>
#include <functional>
#include "game/GameModel.hpp"
#include "DisplayMessageBuilder.hpp"

struct PlayerInfo {
    std::string playerID;
    networking::Connection clientID;
};

class Command {
public:
    typedef std::function<DisplayMessageBuilder (const std::vector<std::string>& arguments, const PlayerInfo& player)> functionRef;


    /**
     * Type supported by game controller for calling appropriate game functions.
     * Final parameter is a function pointer to the method that provides the functionality.
     * @param type
     * @param keyword
     * @param method
     * @return
     */
    Command(const std::string& keyword, const functionRef& method) : text{keyword}, method{method} {};
    const std::string& getKeyword() const;
    const functionRef& getMethod() const;

private:
    std::string text;
    functionRef method;
};


#endif //ADVENTURE2016_COMMAND_H
