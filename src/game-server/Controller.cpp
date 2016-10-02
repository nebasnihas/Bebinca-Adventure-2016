#include <clocale>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "Controller.hpp"
#include "Authenticator.hpp"
#include "game/protocols/Message.hpp"
#include "StringUtils.hpp"
#include "game/protocols/Message.hpp"

using namespace networking;
using namespace std::placeholders;


Controller::Controller() {
    registerForPlayerCommand(Command{"look", std::bind(&Controller::look, this, _1, _2, _3, _4)});
    registerForPlayerCommand(Command{"say", std::bind(&Controller::sayHandler, this, _1, _2, _3, _4)});
}

void Controller::processCommand(const std::string& text, const Connection& client, GameModel& gameModel, MessageSender& messageSender) {
    auto serverMessage = protocols::readMessage(text);
    auto separated = separateFirstWord(serverMessage.messageBody);

    auto commandType = serverMessage.type;
    auto command = separated.first;
    auto commandArgs = splitString(separated.second);

    //get the correct command map
    CommandMap map;
    if (commandType == protocols::MessageType::SERVER_COMMAND) {
        map = systemCommandMap;
    } else if (commandType == protocols::MessageType::USER_COMMAND) {
        map = playerCommandMap;
    }

    auto it = map.find(command);
    if (it != map.end()) {
        auto handler = it->second.getMethod();
        auto playerId = clientToPlayerMap[client];
        handler(commandArgs, PlayerInfo{playerId, client}, gameModel, messageSender);
    } else {
        messageSender.sendMessage(protocols::createMessage(protocols::MessageType::CLIENT_DISPLAY_MESSAGE, "<" + command + "> is an invalid command.\n"), client);
    }
}

void Controller::registerForPlayerCommand(const Command &command) {
    playerCommandMap.insert(std::make_pair(command.getKeyword(), command));
}

void Controller::registerForSystemCommand(const Command &command) {
    systemCommandMap.insert(std::make_pair(command.getKeyword(), command));
}

void Controller::look(const std::vector<std::string>& targets, const PlayerInfo& player, GameModel& gameModel, MessageSender& messageSender) {
//    auto character = gameModel.getCharacterByID(playerID);
//    std::string areaDescription = gameModel.getAreaDescription(character->getAreaID());

    messageSender.sendMessage(protocols::createMessage(protocols::MessageType::CLIENT_DISPLAY_MESSAGE, "You look around...\n"), player.clientID);
}

void Controller::sayHandler(const std::vector<std::string> &targets, const PlayerInfo& player, GameModel &gameModel, MessageSender &messageSender)
{
    std::string str;
    for (const auto& s : targets) {
        str += s + " ";
    }

    messageSender.sendMessage(protocols::createMessage(protocols::MessageType::CLIENT_DISPLAY_MESSAGE, str + "\n"));
}

void Controller::addNewPlayer(const PlayerInfo &player) {
    clientToPlayerMap.insert(std::make_pair(player.clientID, player.playerID));
    playerToClientMap.insert(std::make_pair(player.playerID, player.clientID));
}





