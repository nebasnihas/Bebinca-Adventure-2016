#include <commands/DisplayMessageBuilder.hpp>
#include "LookCommand.hpp"

LookCommand::LookCommand(GameModel& gameModel) : gameModel{gameModel} {}

std::unique_ptr<MessageBuilder> LookCommand::execute(const gsl::span<std::string, -1> arguments,
                                                     const PlayerInfo& player) {
    auto areaID = gameModel.getCharacterByID(player.playerID)->getAreaID();
    std::string description;

    if (arguments.empty()) {
        description = gameModel.getAreaDescription(areaID);
    } else {
        auto targetEntity = arguments[0];
        description = gameModel.getEntityDescription(areaID, targetEntity);
    }

    return DisplayMessageBuilder{description}
            .addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}
