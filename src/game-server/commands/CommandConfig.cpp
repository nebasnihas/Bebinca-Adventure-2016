#include "CommandConfig.hpp"
#include <glog/logging.h>

namespace {
const std::string COMMAND_CONFIGURATION_KEY = "commands";
const std::string DESC_KEY = "desc";
const std::string USAGE_KEY = "usage";
const std::string BINDINGS_KEY = "bindings";

YAML::Node getConfigNodeForCommand(const std::string& commandId, const YAML::Node& rootNode) {
    const std::string key = "command-" + commandId;

    const auto configNode = rootNode[key];
    CHECK(configNode) << "Command configuration doesn't exist for: " << commandId;
    LOG(INFO) << "Configuring options for command: " << commandId;

    return configNode;
}

void addDesc(CommandHandle& cmd, const YAML::Node& node) {
    const auto descNode = node[DESC_KEY];
    if (descNode) {
        LOG(INFO) << "Found description for command: " << cmd.getId();
        cmd.setDescription(descNode.as<std::string>());
    } else {
        LOG(WARNING) << "No description for command: " << cmd.getId();
    }
}

void addUsage(CommandHandle& cmd, const YAML::Node& node) {
    const auto usageNode = node[USAGE_KEY];
    if (usageNode) {
        LOG(INFO) << "Found usage for command: " << cmd.getId();
        cmd.setUsage(usageNode.as<std::string>());
    } else {
        LOG(WARNING) << "No usage found for command: " << cmd.getId();
    }
}

std::vector<std::string> getInputBindings(const std::string& commandId, const YAML::Node node) {
    const auto inputBindingsNode = node[BINDINGS_KEY];
    CHECK(inputBindingsNode) << "No bindings configuration found for command: " << commandId;

    auto inputBindings = inputBindingsNode.as<std::vector<std::string>>();
    CHECK(!inputBindings.empty()) << "No bindings for command: " << commandId;

    LOG(INFO) << "Found bindings for command:" << commandId;

    return inputBindings;
}
}

CommandConfig::CommandConfig(const std::string& commandConfigFileName) : fileName{commandConfigFileName} {
    loadFile();
}

void CommandConfig::loadFile() {
    auto contents = YAML::LoadFile(fileName);
    root = contents[COMMAND_CONFIGURATION_KEY];
    CHECK(root) << "Inavlid configuration file for commands: " << fileName;
}

using retVector = std::vector<std::pair<std::string, std::shared_ptr<CommandHandle>>>;
std::vector<std::pair<std::string, std::shared_ptr<CommandHandle>>> CommandConfig::createInputBindingsForCommand(
        const std::string& commandId, Command& command) const {
    auto cmd = std::make_shared<CommandHandle>(commandId, command);

    auto node = getConfigNodeForCommand(commandId, root);
    addDesc(*cmd, node);
    addUsage(*cmd, node);

    auto inputBindings = getInputBindings(commandId, node);
    retVector returnVal;
    returnVal.reserve(inputBindings.size());
    std::transform(inputBindings.begin(), inputBindings.end(), std::back_inserter(returnVal), [&cmd, &commandId](auto binding) {
        LOG(INFO) << "Adding binding " << binding << " to command: " << commandId;
        cmd->addInputBinding(binding);
        return std::make_pair(binding, cmd);
    });

    return returnVal;
}