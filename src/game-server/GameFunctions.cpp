
#include <game/protocols/Message.hpp>
#include "GameFunctions.hpp"

using namespace std;

void GameFunctions::look(const std::vector<std::string>& targets, const PlayerInfo& player, GameModel& gameModel, MessageSender& messageSender) {
    auto areaID = gameModel.getCharacterByID(player.playerID)->getAreaID();
    string description;
    if (targets.empty()) {
        description = gameModel.getAreaDescription(areaID);
    }
    else {
        auto targetEntity = targets[0];
        description = gameModel.getEntityDescription(areaID, targetEntity);
    }
    messageSender.sendMessage(protocols::createMessage(protocols::MessageType::CLIENT_DISPLAY_MESSAGE, description), player.clientID);
}

void GameFunctions::move(const std::vector<std::string>& targets, const PlayerInfo& player, GameModel& gameModel, MessageSender& messageSender) {

}

void GameFunctions::listPlayers(const std::vector<std::string>& targets, const PlayerInfo& player, GameModel& gameModel, MessageSender& messageSender) {

}

void GameFunctions::listExits(const std::vector<std::string>& targets, const PlayerInfo& player, GameModel& gameModel, MessageSender& messageSender) {

}