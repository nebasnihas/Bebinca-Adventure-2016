#include <commands/DisplayMessageBuilder.hpp>
#include "ListExitsCommand.hpp"

ListExitsCommand::ListExitsCommand(GameModel& gameModel) : gameModel{gameModel} {}

std::unique_ptr<MessageBuilder> ListExitsCommand::execute(const gsl::span<std::string, -1> arguments,
                                                          const PlayerInfo& player) {
    auto areaID = gameModel.getCharacterByID(player.playerID)->getAreaID();
    auto connectedAreaMap = *gameModel.getConnectedAreas(areaID);
    //TODO: extract into string resource file
    std::string message = "Exits:\n";
    for (auto mapEntry: connectedAreaMap) {
        message += mapEntry.first + "\n";
    }

    return DisplayMessageBuilder{message}
            .addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}
