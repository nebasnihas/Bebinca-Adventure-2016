#include "CommandConfig.hpp"
#include <glog/logging.h>

namespace {
const std::string COMMAND_CONFIGURATION_KEY = "commands";
const std::string DESC_KEY = "desc";
const std::string USAGE_KEY = "usage";
const std::string BINDINGS_KEY = "bindings";
const std::string ROLE_KEY = "role";

YAML::Node getConfigNodeForCommand(const std::string& commandId, const YAML::Node& rootNode) {
    const std::string key = "command-" + commandId;

    const auto configNode = rootNode[key];
    CHECK(configNode) << "Command configuration doesn't exist for: " << commandId;
    LOG(INFO) << "Configuring options for command: " << commandId;

    return configNode;
}

void addDesc(CommandHandle& cmd, const YAML::Node& node) {
    const auto& descNode = node[DESC_KEY];
    if (descNode) {
        LOG(INFO) << "Found description for command: " << cmd.getId();
        cmd.setDescription(descNode.as<std::string>());
    } else {
        LOG(WARNING) << "No description for command: " << cmd.getId();
    }
}

void addUsage(CommandHandle& cmd, const YAML::Node& node) {
    const auto& usageNode = node[USAGE_KEY];
    if (usageNode) {
        LOG(INFO) << "Found usage for command: " << cmd.getId();
        cmd.setUsage(usageNode.as<std::string>());
    } else {
        LOG(WARNING) << "No usage found for command: " << cmd.getId();
    }
}

std::vector<std::string> getInputBindings(const std::string& commandId, const YAML::Node node) {
    const auto& inputBindingsNode = node[BINDINGS_KEY];
    CHECK(inputBindingsNode) << "No bindings configuration found for command: " << commandId;

    auto inputBindings = inputBindingsNode.as<std::vector<std::string>>();
    CHECK(!inputBindings.empty()) << "No bindings for command: " << commandId;

    LOG(INFO) << "Found bindings for command:" << commandId;

    return inputBindings;
}

const static std::unordered_map<std::string, PlayerRole> stringToRole = {
        {"user", PlayerRole::NORMAL},
        {"admin", PlayerRole::ADMIN},
};

void addRole(CommandHandle& cmd, const YAML::Node& node) {
    const auto& roleNode = node[ROLE_KEY];
    CHECK(roleNode) << "No role defined for command: " << cmd.getId();

    auto role = roleNode.as<std::string>();
    auto it = stringToRole.find(role);
    CHECK(it != stringToRole.end()) << "Not a valid role: " << role;

    LOG(INFO) << "Adding role: " << role << " to command";
    cmd.setRole(it->second);
}
}

CommandConfig::CommandConfig(const std::string& commandConfigFileName) {
    addFileSource(commandConfigFileName);
}

CommandConfig::CommandConfig(const YAML::Node& commandConfigNode) {
    addNodeSource(commandConfigNode);
}


CommandHandle CommandConfig::createInputBindingsForCommand(const std::string& commandId,
                                                           Command& command) const {
    CommandHandle cmd{commandId, command};

    auto node = getConfigNodeForCommand(commandId, root);
    addDesc(cmd, node);
    addUsage(cmd, node);
    addRole(cmd, node);

    auto inputBindings = getInputBindings(commandId, node);
    for (const auto& binding : inputBindings) {
        LOG(INFO) << "Adding binding " << binding << " to command: " << commandId;
        cmd.addInputBinding(binding);
    }

    return cmd;
}

void CommandConfig::addFileSource(const std::string& fileName) {
    auto contents = YAML::LoadFile(fileName);
    CHECK(contents) << "Inavalid configuration file for commands from file: " << fileName;
    addNodeSource(contents);

    fileSources.insert(fileName);
}

void CommandConfig::addNodeSource(const YAML::Node& node) {
    auto newNode = node[COMMAND_CONFIGURATION_KEY];
    CHECK(newNode) << "Invalid configuration file for commands, cannot find the key " << COMMAND_CONFIGURATION_KEY;

    for (const auto& it : newNode) {
        auto key = it.first.as<std::string>();
        LOG_IF(WARNING, root[key]) << "Duplicate when reading inserting configuration options for commands: " << key;
        root[key] = it.second;
    }
}

void CommandConfig::reloadFromSources() {
    root.reset();
    //TODO reload from files and nodes?
}
