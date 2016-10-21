#ifndef ADVENTURE2016_COMMAND_H
#define ADVENTURE2016_COMMAND_H


#include <vector>
#include <string>
#include <functional>
#include "game/GameModel.hpp"
#include "MessageBuilder.hpp"
#include <memory>

struct PlayerInfo {
    std::string playerID;
    networking::Connection clientID;
};

class Command {
public:
    typedef std::function<std::unique_ptr<MessageBuilder> (const std::vector<std::string>& arguments, const PlayerInfo& player)> functionRef;


    /**
     * Type supported by game controller for calling appropriate game functions.
     * Final parameter is a function pointer to the method that provides the functionality.
     * @param type
     * @param keyword
     * @param method
     * @return
     */
    Command(const std::string& keyword, const functionRef& method) : keyword{keyword}, method{method} {};
    const std::string& getKeyword() const;
    const functionRef& getMethod() const;
    const std::string& getDesc() const;
    const std::string& getUsage() const;
    const std::vector<std::string> getAliases() const;

    void setDesc(const std::string& desc);
    void setUsage(const std::string& usage);
    void addAlias(const std::string& alias);
private:
    std::string keyword;
    functionRef method;
    std::string desc = "No Desc";
    //just a usage message, maybe do actual parsing based on this later
    std::string usage = "No usage";
    std::vector<std::string> aliases;
};


#endif //ADVENTURE2016_COMMAND_H
