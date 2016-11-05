#include <commands/DisplayMessageBuilder.hpp>
#include <GameStrings.hpp>
#include "ShoutCommand.hpp"

ShoutCommand::ShoutCommand(GameModel& gameModel, Controller& controller) : gameModel{gameModel}, controller{controller} {}

std::unique_ptr<MessageBuilder> ShoutCommand::execute(const gsl::span<std::string, -1> arguments,
                                                      const PlayerInfo& player) {
    std::string message = GameStrings::get(GameStringKeys::AREA_CHANNEL) + " ";
    for (const auto& target : arguments) {
        message += target + " ";
    }
    auto areaID = gameModel.getCharacterByID(player.playerID)->getAreaID();

    std::vector<networking::Connection> localClients;
    for (const auto &character: gameModel.getCharacterIDsInArea(areaID)) {
        localClients.push_back(controller.getClientID(character).get());
    }

    return DisplayMessageBuilder{message}
            .addClients(localClients)
            .setSender(player.playerID);
}
