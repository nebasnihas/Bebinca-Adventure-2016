#include <commands/DisplayMessageBuilder.hpp>
#include "GameStrings.hpp"
#include "ShoutCommand.hpp"

ShoutCommand::ShoutCommand(GameModel& gameModel, Controller& controller) : gameModel{gameModel}, controller{controller} {}

std::unique_ptr<MessageBuilder> ShoutCommand::execute(const gsl::span<std::string, -1> arguments,
                                                      const PlayerInfo& player) {
    std::string message = GameStrings::get(GameStringKeys::AREA_CHANNEL) + " ";
    for (const auto& target : arguments) {
        message += target + " ";
    }
	gameModel.sendLocalMessage(player.playerID, message);
    return DisplayMessageBuilder{message};
}
