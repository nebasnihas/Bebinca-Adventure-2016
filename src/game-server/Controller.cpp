#include <clocale>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <game/protocols/DisplayMessage.hpp>
#include "Controller.hpp"
#include "Authenticator.hpp"
#include "game/protocols/RequestMessage.hpp"
#include "StringUtils.hpp"
#include "game/protocols/RequestMessage.hpp"

using namespace networking;
using namespace std::placeholders;


Controller::Controller() {
    registerForPlayerCommand(Command{"look", std::bind(&Controller::look, this, _1, _2, _3, _4)});
    registerForPlayerCommand(Command{"say", std::bind(&Controller::sayHandler, this, _1, _2, _3, _4)});
}

void Controller::processCommand(const protocols::PlayerCommand& command, const Connection& client, GameModel& gameModel,
                                MessageSender& messageSender) {

    auto cmd = command.command;
    auto cmdArgs = splitString(command.arguments);


    auto it = playerCommandMap.find(cmd);
    if (it != playerCommandMap.end()) {
        auto handler = it->second.getMethod();
        auto playerId = clientToPlayerMap[client];
        handler(cmdArgs, PlayerInfo{playerId, client}, gameModel, messageSender);
    } else {
        auto response = protocols::createDisplayResponseMessage(protocols::DisplayMessage{"<" + cmd + "> is an invalid command."});
        messageSender.sendMessage(protocols::serializeResponseMessage(response), client);
    }
}

void Controller::registerForPlayerCommand(const Command &command) {
    playerCommandMap.insert(std::make_pair(command.getKeyword(), command));
}


void Controller::look(const std::vector<std::string>& targets, const PlayerInfo& player, GameModel& gameModel, MessageSender& messageSender) {
//    auto character = gameModel.getCharacterByID(playerID);
//    std::string areaDescription = gameModel.getAreaDescription(character->getAreaID());

    auto response = protocols::createDisplayResponseMessage(protocols::DisplayMessage{"You look around..."});
    messageSender.sendMessage(protocols::serializeResponseMessage(response), player.clientID);
}

void Controller::sayHandler(const std::vector<std::string> &targets, const PlayerInfo& player, GameModel &gameModel, MessageSender &messageSender)
{
    std::string str;
    for (const auto& s : targets) {
        str += s + " ";
    }

    auto response = protocols::createDisplayResponseMessage(protocols::DisplayMessage{str});
    messageSender.sendMessage(protocols::serializeResponseMessage(response));
}

void Controller::addNewPlayer(const PlayerInfo &player) {
    clientToPlayerMap.insert(std::make_pair(player.clientID, player.playerID));
    playerToClientMap.insert(std::make_pair(player.playerID, player.clientID));
}





