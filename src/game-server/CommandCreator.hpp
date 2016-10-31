#ifndef ADVENTURE2016_COMMANDCREATOR_HPP
#define ADVENTURE2016_COMMANDCREATOR_HPP

#include <string>
#include "Command.hpp"
#include "yaml-cpp/yaml.h"
#include <vector>

//TODO better name
class CommandCreator {
public:
    CommandCreator(const std::string& commandConfigFileName);

    //TODO commands will be refactored, for now we reuse the command class
    std::vector<std::pair<std::string, std::shared_ptr<Command>>> createBindingsFromCommand(const Command& command) const;

private:
    void loadFile();

    std::string fileName;
    YAML::Node root;
};


#endif //ADVENTURE2016_COMMANDCREATOR_HPP
