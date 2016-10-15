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

DisplayMessageBuilder Controller::processCommand(const protocols::PlayerCommand& command, const Connection& client)
{
    auto cmd = command.command;
    auto cmdArgs = splitString(command.arguments);

    auto it = playerCommandMap.find(cmd);
    if (it == playerCommandMap.end()) {
        return DisplayMessageBuilder::createMessage("<" + cmd + "> is an invalid command.")
                .addClient(client)
                .setSender(DisplayMessageBuilder::SENDER_SERVER);
    }
    auto handler = it->second.getMethod();

    auto playerId = playerMap.right.find(client)->second;
    return handler(cmdArgs, PlayerInfo{playerId, client});
}

void Controller::registerCommand(const Command &command) {
    playerCommandMap.insert(std::make_pair(command.getKeyword(), command));
}

void Controller::addNewPlayer(const PlayerInfo &player) {
    playerMap.insert(PlayerMapPair{player.playerID, player.clientID});
    gameModel.createCharacter(player.playerID, player.playerID);
}

const std::vector<Connection>& Controller::getAllClients() const {
    return allClients;
}

GameModel& Controller::getGameModel() const {
    return gameModel;
}





