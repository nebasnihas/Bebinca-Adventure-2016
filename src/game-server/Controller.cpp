#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <game/protocols/DisplayMessage.hpp>
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
    auto handler = it->second.getMethod();

    auto playerID = getPlayerID(client).get();
    return handler(cmdArgs, PlayerInfo{playerID, client});
}

void Controller::registerCommand(const Command& command) {
    playerCommandMap.insert(std::make_pair(command.getKeyword(), command));
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
    auto player = getPlayerID(clientID).get();

    playerMap.right.erase(clientID);
    allClients.erase(std::remove(allClients.begin(), allClients.end(), clientID), allClients.end());
    //TODO remove from game model

    auto outMsg = DisplayMessageBuilder::createMessage("Player <" + player + "> has disconnected.\n")
            .setSender(DisplayMessageBuilder::SENDER_SERVER)
            .addClients(getAllClients()).buildMessages();
    server.send(outMsg);
}

const boost::optional<Connection> Controller::getClientID(const std::string& playerID) const {
    boost::optional<Connection> clientID;
    if (playerMap.left.find(playerID) == playerMap.left.end()) {
        return clientID;
    }
    clientID = playerMap.left.find(playerID)->second;
    return clientID;
}

const boost::optional<std::string> Controller::getPlayerID(const networking::Connection& clientID) const {
    boost::optional<std::string> playerID;
    if (playerMap.right.find(clientID) == playerMap.right.end()) {
        return playerID;
    }
    playerID = playerMap.right.find(clientID)->second;
    return playerID;
}

void Controller::disconnectPlayer(const std::string& playerID) {
    auto clientId = getClientID(playerID);
    server.disconnect(clientId.get());
}






