#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <game/protocols/DisplayMessage.hpp>
#include <glog/logging.h>
#include "Controller.hpp"
#include "Authenticator.hpp"
#include "StringUtils.hpp"
#include "DisplayMessageBuilder.hpp"
#include "CommandCreator.hpp"
#include <functional>
#include <unordered_set>

using namespace networking;
using namespace std::placeholders;

Controller::Controller(GameModel& gameModel, Server& server, const CommandCreator& commandCreator)
        : gameModel{gameModel}, server{server}, commandCreator{commandCreator} {
    registerCommand(Command{"help", std::bind(&Controller::help, this, std::placeholders::_1, std::placeholders::_2)});
}

std::unique_ptr<MessageBuilder> Controller::processCommand(const protocols::PlayerCommand& command,
                                                           const Connection& client) {
    auto cmd = command.command;
    auto cmdArgs = splitString(command.arguments);

    auto it = playerCommandMap.find(cmd);
    if (it == playerCommandMap.end()) {
        return DisplayMessageBuilder::createMessage("<" + cmd + "> is an invalid command. Type help")
                .addClient(client)
                .setSender(DisplayMessageBuilder::SENDER_SERVER);
    }
    auto handler = it->second->getMethod();

    auto playerID = getPlayerID(client);
    return handler(cmdArgs, PlayerInfo{playerID, client});
}

void Controller::registerCommand(const Command& command) {
    auto bindings = commandCreator.createInputBindingsForCommand(command);
    playerCommandMap.insert(bindings.begin(), bindings.end());
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
        message += "\t" + getCommandBindingsHelpMessage(command);
    }

    return DisplayMessageBuilder::createMessage(message).addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}

std::unique_ptr<MessageBuilder> Controller::allCommandsHelp(const networking::Connection& clientID) {
    //Get al unique commands
    std::unordered_set<std::string> commands;
    for (const auto& it : playerCommandMap) {
        commands.insert(it.second->getKeyword());
    }

    std::string message = "List of available commands. Type help <command> for more information\n";
    for (const auto& cmd : commands) {
        message += "Command: " + cmd;
        message += " - " + getCommandBindingsHelpMessage(cmd);
        message += "\n";
    }

    return DisplayMessageBuilder::createMessage(message).addClient(clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}

std::string Controller::getCommandBindingsHelpMessage(const std::string command) {
    std::string message;
    message += "Bindings:[";
    message += boost::algorithm::join(playerCommandMap[command]->getBindings(), ",");
    message += "]";

    return message;
}


