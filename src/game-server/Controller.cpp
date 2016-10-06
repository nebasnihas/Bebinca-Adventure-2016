#include <clocale>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <game/protocols/DisplayMessage.hpp>
#include "Controller.hpp"
#include "Authenticator.hpp"
#include "game/protocols/RequestMessage.hpp"
#include "StringUtils.hpp"
#include "game/protocols/RequestMessage.hpp"
#include "GameFunctions.hpp"

using namespace networking;
using namespace std::placeholders;


Controller::Controller(GameModel& gameModel, std::vector<Connection>& allClients) : gameModel{gameModel}, allClients{allClients} {
    GameFunctions gameFunctions{*this};
}

DisplayMessageBuilder Controller::processCommand(const protocols::PlayerCommand& command, const Connection& client)
{
    auto cmd = command.command;
    auto cmdArgs = splitString(command.arguments);


    auto it = playerCommandMap.find(cmd);
    if (it != playerCommandMap.end()) {
        auto handler = it->second.getMethod();
        auto playerId = clientToPlayerMap[client];
        return handler(cmdArgs, PlayerInfo{playerId, client});
    } else {
        return DisplayMessageBuilder::createMessage("<" + cmd + "> is an invalid command.")
                .addClient(client)
                .setSender(DisplayMessageBuilder::SENDER_SERVER);
    }
}

void Controller::registerCommand(const Command &command) {
    playerCommandMap.insert(std::make_pair(command.getKeyword(), command));
}

void Controller::addNewPlayer(const PlayerInfo &player) {
    clientToPlayerMap.insert(std::make_pair(player.clientID, player.playerID));
    playerToClientMap.insert(std::make_pair(player.playerID, player.clientID));
    std::cout << allClients.front().id << endl;
}

std::vector<Connection>& Controller::getAllClients() const {
    return allClients;
}

GameModel& Controller::getGameModel() const {
    return gameModel;
}





