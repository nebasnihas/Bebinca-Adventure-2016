#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <game/protocols/DisplayMessage.hpp>
#include <glog/logging.h>
#include "Controller.hpp"
#include "Authenticator.hpp"
#include "StringUtils.hpp"
#include "DisplayMessageBuilder.hpp"
#include <functional>
#include <unordered_set>

using namespace networking;
using namespace std::placeholders;

Controller::Controller(GameModel& gameModel, networking::Server& server, const YAML::Node& commandBindingsNode)
        : gameModel{gameModel}, server{server}, bindings{commandBindingsNode} {
    registerCommand(Command{"help", std::bind(&Controller::help, this, std::placeholders::_1, std::placeholders::_2)});
}

std::unique_ptr<MessageBuilder> Controller::processCommand(const protocols::PlayerCommand& command,
                                                           const Connection& client) {
    auto cmd = command.command;
    auto cmdArgs = splitString(command.arguments);

    auto it = playerCommandMap.find(cmd);
    if (it == playerCommandMap.end()) {
        return DisplayMessageBuilder::createMessage("<" + cmd + "> is an invalid command.")
                .addClient(client)
                .setSender(DisplayMessageBuilder::SENDER_SERVER);
    }
    auto handler = it->second->getMethod();

    auto playerID = getPlayerID(client);
    return handler(cmdArgs, PlayerInfo{playerID, client});
}

void Controller::registerCommand(const Command& command) {
    auto cmd = std::make_shared<Command>(command);
    playerCommandMap.emplace(cmd->getKeyword(), cmd);

    //no configuration
    if (!bindings) {
        return;
    }

    //read configuration and setup bindings
    const std::string DESC_KEY = "desc";
    const std::string USAGE_KEY = "usage";
    const std::string ALIAS_KEY = "alias";
    const std::string key = "command-" + command.getKeyword();

    //Get the yaml node for the binding configuration for this command
    const auto& bindingNode = bindings[key];
    if (!bindingNode) {
        LOG(INFO) << "No bindings found for command: " << cmd->getKeyword();
        return;
    }
    LOG(INFO) << "Configuring options for command: " << cmd->getKeyword();

    //Get the description of this command
    if (const auto& descNode = bindingNode[DESC_KEY]) {
        LOG(INFO) << "Found description for command: " << cmd->getKeyword();
        cmd->setDesc(descNode.as<std::string>());
    } else {
        LOG(WARNING) << "No description for command: " << cmd->getKeyword();
    }

    //Get the usage for this command
    if (const auto& usageNode = bindingNode[USAGE_KEY]) {
        LOG(INFO) << "Found usage for command: " << cmd->getKeyword();
        cmd->setUsage(usageNode.as<std::string>());
    } else {
        LOG(WARNING) << "No usage found for command: " << cmd->getKeyword();
    }

    //Add each alias to command map
    const auto& aliasNode = bindingNode[ALIAS_KEY];
    if (!aliasNode) {
        LOG(INFO) << "No aliases found for command: " << cmd->getKeyword();
        return;
    }

    LOG(INFO) << "Found aliases for command:" << cmd->getKeyword();
    for (const auto& alias : aliasNode.as<std::vector<std::string>>()) {
        if (playerCommandMap.count(alias) == 1) {
            LOG(WARNING) << "Alias: " << alias << " already exists.";
            break;
        }

        LOG(INFO) << "Adding alias " << alias << " to command: " << cmd->getKeyword();
        cmd->addAlias(alias);
        playerCommandMap.emplace(alias, cmd);
    }
}

void Controller::addNewPlayer(const PlayerInfo& player) {
    playerMap.insert(PlayerMapPair{player.playerID, player.clientID});
    allClients.push_back(player.clientID);
    gameModel.createCharacter(player.playerID, player.playerID);

    auto outMsg = DisplayMessageBuilder::createMessage("Player <" + player.playerID + "> has joined.")
            .setSender(DisplayMessageBuilder::SENDER_SERVER)
            .addClients(getAllClients())
            .buildMessages();
    server.send(outMsg);
}

const std::vector<Connection>& Controller::getAllClients() const {
    return allClients;
}

GameModel& Controller::getGameModel() const {
    return gameModel;
}

void Controller::removePlayer(const networking::Connection& clientID) {
    auto player = getPlayerID(clientID);

    playerMap.right.erase(clientID);
    allClients.erase(std::remove(allClients.begin(), allClients.end(), clientID), allClients.end());
    //TODO remove from game model

    auto outMsg = DisplayMessageBuilder::createMessage("Player <" + player + "> has disconnected.")
            .setSender(DisplayMessageBuilder::SENDER_SERVER)
            .addClients(getAllClients()).buildMessages();
    server.send(outMsg);
}

const Connection& Controller::getClientID(const std::string& playerID) const {
    return playerMap.left.find(playerID)->second;
}

const std::string& Controller::getPlayerID(const networking::Connection& clientID) const {
    return playerMap.right.find(clientID)->second;
}

void Controller::disconnectPlayer(const std::string& playerID) {
    auto clientId = getClientID(playerID);
    server.disconnect(clientId);
}

std::unique_ptr<MessageBuilder> Controller::help(const std::vector<std::string>& targets, const PlayerInfo& player) {
    //TODO these command functions can get long, maybe they each need a class?

    if (targets.empty()) {
        //return help for all commands
        return allCommandsHelp(player.clientID);
    }

    //return help for the command
    std::string message;
    auto command = targets[0];

    auto it = playerCommandMap.find(command);
    if (it == playerCommandMap.end()) {
        message = "Command <" + command + "> not found.";
    } else {
        message = "Help for command <" + it->second->getKeyword() + ">\n";
        message += "\tDescription: " + it->second->getDesc() + "\n";
        message += "\tUsage: " + command + " " + it->second->getUsage() + "\n";
        message += "\t" + getCommandAliasesHelpMessage(command);
    }

    return DisplayMessageBuilder::createMessage(message).addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}

std::unique_ptr<MessageBuilder> Controller::allCommandsHelp(const networking::Connection& clientID) {
    std::unordered_set<std::string> commands;
    for (const auto& it : playerCommandMap) {
        commands.insert(it.second->getKeyword());
    }

    std::string message = "Available commands. Type help <command> for more information\n";
    for (const auto& cmd : commands) {
        message += cmd;
        if (!playerCommandMap[cmd]->getAliases().empty()) {
            message += " - " + getCommandAliasesHelpMessage(cmd);
        }
        message += "\n";
    }

    return DisplayMessageBuilder::createMessage(message).addClient(clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}

std::string Controller::getCommandAliasesHelpMessage(const std::string command) {
    std::string message;
    message += "Aliases:[";
    message += boost::algorithm::join(playerCommandMap[command]->getAliases(), ",");
    message += "]";

    return message;
}


