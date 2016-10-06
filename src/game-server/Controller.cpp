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
    registerForPlayerCommand(Command{"look", std::bind(&Controller::lookHandler, this, _1, _2, _3, _4)});
    registerForPlayerCommand(Command{"say", std::bind(&Controller::sayHandler, this, _1, _2, _3, _4)});
}

DisplayMessageBuilder Controller::processCommand(const protocols::PlayerCommand& command, const Connection& client, GameModel& gameModel, std::vector<Connection>& allClients)
{
    auto cmd = command.command;
    auto cmdArgs = splitString(command.arguments);


    auto it = playerCommandMap.find(cmd);
    if (it != playerCommandMap.end()) {
        auto handler = it->second.getMethod();
        auto playerId = clientToPlayerMap[client];
        return handler(cmdArgs, PlayerInfo{playerId, client}, gameModel, allClients);
    } else {
        return DisplayMessageBuilder::createMessage("<" + cmd + "> is an invalid command.")
                .addClient(client)
                .setSender(DisplayMessageBuilder::SENDER_SERVER);
    }
}

void Controller::registerForPlayerCommand(const Command &command) {
    playerCommandMap.insert(std::make_pair(command.getKeyword(), command));
}


DisplayMessageBuilder Controller::lookHandler(const vector<string>& arguments, const PlayerInfo& player, GameModel& gameModel, const std::vector<Connection>& allClients)
{
//    auto character = gameModel.getCharacterByID(playerID);
//    std::string areaDescription = gameModel.getAreaDescription(character->getAreaID());

    return DisplayMessageBuilder::createMessage("You look around...").addClient(player.clientID).setSender(DisplayMessageBuilder::SENDER_SERVER);
}

DisplayMessageBuilder Controller::sayHandler(const vector<string>& arguments, const PlayerInfo& player, GameModel& gameModel, const std::vector<Connection>& allClients)
{
    std::string str;
    for (const auto& s : arguments) {
        str += s + " ";
    }

    return DisplayMessageBuilder::createMessage(str).addClients(allClients).setSender(player.playerID);
}

void Controller::addNewPlayer(const PlayerInfo &player) {
    clientToPlayerMap.insert(std::make_pair(player.clientID, player.playerID));
    playerToClientMap.insert(std::make_pair(player.playerID, player.clientID));
}





