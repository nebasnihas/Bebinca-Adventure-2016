#include <commands/DisplayMessageBuilder.hpp>
#include "MoveCommand.hpp"

MoveCommand::MoveCommand(GameModel& gameModel) : gameModel{gameModel} {}

std::unique_ptr<MessageBuilder> MoveCommand::execute(const gsl::span<std::string, -1> arguments,
                                                     const PlayerInfo& player) {
    std::string message;
    if (gameModel.moveCharacter(player.playerID, arguments[0])) {
        auto areaID = gameModel.getCharacterByID(player.playerID)->getAreaID();
        message = gameModel.getAreaDescription(areaID);
    } else {
        //TODO: extract into string resource
        message = "Area not found";
    }

    return DisplayMessageBuilder{message}
            .addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}


