#include <commands/DisplayMessageBuilder.hpp>
#include <boost/algorithm/string.hpp>
#include "ListPlayersCommand.hpp"

ListPlayersCommand::ListPlayersCommand(GameModel& gameModel) : gameModel{gameModel} {}

std::unique_ptr<MessageBuilder> ListPlayersCommand::execute(const gsl::span<std::string, -1> arguments,
                                                            const PlayerInfo& player) {
    auto areaID = gameModel.getCharacterByID(player.playerID)->getAreaID();
    //TODO: extract into string resource file
    std::string message = "Players in the area:\n";
    for (const auto& playerID: gameModel.getCharacterIDsInArea(areaID)) {
        message += gameModel.getCharacterByID(playerID)->getName() + " \n";
    }

    return DisplayMessageBuilder{message}
            .addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}


