#include <commands/DisplayMessageBuilder.hpp>
#include <GameStrings.hpp>
#include "MoveCommand.hpp"

MoveCommand::MoveCommand(GameModel& gameModel) : gameModel{gameModel} {}

std::unique_ptr<MessageBuilder> MoveCommand::execute(const gsl::span<std::string, -1> arguments,
                                                     const PlayerInfo& player) {
    std::string message;
    if (gameModel.moveCharacter(player.playerID, arguments[0])) {
        auto areaID = gameModel.getCharacterByID(player.playerID)->getAreaID();
        message = gameModel.getAreaDescription(areaID);
    } else {
        message = GameStrings::get(GameStringKeys::INVALID_DIR);
    }

    return DisplayMessageBuilder{message}
            .addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}


