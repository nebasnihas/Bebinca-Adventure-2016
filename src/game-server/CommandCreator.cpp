#include "CommandCreator.hpp"
#include <glog/logging.h>

namespace {
const std::string COMMAND_CONFIGURATION_KEY = "commands";
const std::string DESC_KEY = "desc";
const std::string USAGE_KEY = "usage";
const std::string BINDINGS_KEY = "bindings";
}


CommandCreator::CommandCreator(const std::string& commandConfigFileName) : fileName{commandConfigFileName} {
    loadFile();
}

void CommandCreator::loadFile() {
    auto contents = YAML::LoadFile(fileName);
    root = contents[COMMAND_CONFIGURATION_KEY];
    CHECK(root) << "Inavlid configuration file for commands: " << fileName;
}

using retVector = std::vector<std::pair<std::string, std::shared_ptr<Command>>>;
retVector CommandCreator::createBindingsFromCommand(const Command& command) const {
    auto cmd = std::make_shared<Command>(command);

    const std::string key = "command-" + command.getKeyword();

    //Get the yaml node for the binding configuration for this command
    const auto& configNode = root[key];
    CHECK(configNode) << "Command configuration doesn't exist for: " << cmd->getKeyword();
    LOG(INFO) << "Configuring options for command: " << cmd->getKeyword();

    //Get the description of this command
    const auto& descNode = configNode[DESC_KEY];
    if (descNode) {
        LOG(INFO) << "Found description for command: " << cmd->getKeyword();
        cmd->setDesc(descNode.as<std::string>());
    } else {
        LOG(WARNING) << "No description for command: " << cmd->getKeyword();
    }

    //Get the usage for this command
    const auto& usageNode = configNode[USAGE_KEY];
    if (usageNode) {
        LOG(INFO) << "Found usage for command: " << cmd->getKeyword();
        cmd->setUsage(usageNode.as<std::string>());
    } else {
        LOG(WARNING) << "No usage found for command: " << cmd->getKeyword();
    }

    //Add each binding to command map
    const auto& bindingsNode = configNode[BINDINGS_KEY];
    CHECK(bindingsNode) << "No bindings configuration found for command: " << cmd->getKeyword();

    auto bindings = bindingsNode.as<std::vector<std::string>>();
    CHECK(!bindings.empty()) << "No bindings for command: " << cmd->getKeyword();

    LOG(INFO) << "Found bindings for command:" << cmd->getKeyword();

    retVector returnVal;
    returnVal.reserve(bindings.size());
    std::transform(bindings.begin(), bindings.end(), std::back_inserter(returnVal), [&cmd](auto binding) {
        LOG(INFO) << "Adding binding " << binding << " to command: " << cmd->getKeyword();
        cmd->addBinding(binding);
        return std::make_pair(binding, cmd);
    });

    return returnVal;
}