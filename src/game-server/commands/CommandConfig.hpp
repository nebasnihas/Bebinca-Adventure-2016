#ifndef ADVENTURE2016_COMMANDCREATOR_HPP
#define ADVENTURE2016_COMMANDCREATOR_HPP

#include <string>
#include <vector>
#include "commands/Command.hpp"
#include "yaml-cpp/yaml.h"
#include "CommandHandle.hpp"

//TODO better name
class CommandConfig {
public:
    CommandConfig(const std::string& commandConfigFileName);

    std::vector<std::pair<std::string, std::shared_ptr<CommandHandle>>> createInputBindingsForCommand(
            const std::string& commandId, Command& command) const;

private:
    void loadFile();

    std::string fileName;
    YAML::Node root;
};


#endif //ADVENTURE2016_COMMANDCREATOR_HPP
