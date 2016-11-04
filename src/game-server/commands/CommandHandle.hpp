#ifndef ADVENTURE2016_COMMANDHANDLE_HPP
#define ADVENTURE2016_COMMANDHANDLE_HPP

#include <string>
#include <vector>
#include "Command.hpp"

class CommandHandle {
public:
    CommandHandle(const std::string& id, Command& command);

    Command& getCommand();
    const std::string& getId();
    const std::string& getDescription();
    const std::string& getUsage();
    const std::vector<std::string>& getInputBindings();

    void setDescription(const std::string& description);
    void setUsage(const std::string& usage);
    void addInputBinding(const std::string& inputBinding);

private:
    Command& command;
    std::string id;
    std::string description;
    std::string usage;
    std::vector<std::string> inputBindings;
};


#endif //ADVENTURE2016_COMMANDHANDLE_HPP
