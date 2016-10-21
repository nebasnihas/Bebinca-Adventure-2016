#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <game/protocols/DisplayMessage.hpp>
#include <glog/logging.h>
#include "Controller.hpp"
#include "Authenticator.hpp"
#include "StringUtils.hpp"
#include "DisplayMessageBuilder.hpp"


using namespace networking;
using namespace std::placeholders;

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

    auto outMsg = DisplayMessageBuilder::createMessage("Player <" + player.playerID + "> has joined.\n")
            .setSender(DisplayMessageBuilder::SENDER_SERVER)
            .addClients(getAllClients())
            .buildMessages();;
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

    auto outMsg = DisplayMessageBuilder::createMessage("Player <" + player + "> has disconnected.\n")
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






