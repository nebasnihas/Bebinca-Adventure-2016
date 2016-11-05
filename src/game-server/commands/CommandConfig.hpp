#ifndef ADVENTURE2016_COMMANDCREATOR_HPP
#define ADVENTURE2016_COMMANDCREATOR_HPP

#include <string>
#include <vector>
#include <unordered_set>
#include <Command.hpp>
#include <yaml-cpp/yaml.h>
#include "CommandHandle.hpp"

//TODO better name
class CommandConfig {
public:
    CommandConfig(const std::string& commandConfigFileName);
    CommandConfig(const YAML::Node& commandConfigNode);

    void addFileSource(const std::string &fileName);
    void addNodeSource(const YAML::Node& node);
    void reloadFromSources();

    CommandHandle createInputBindingsForCommand(const std::string& commandId,
                                                Command& command) const;

private:
    std::unordered_set<std::string> fileSources;
    YAML::Node root;
};


#endif //ADVENTURE2016_COMMANDCREATOR_HPP
