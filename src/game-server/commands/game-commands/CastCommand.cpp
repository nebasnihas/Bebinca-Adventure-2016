
#include <commands/DisplayMessageBuilder.hpp>
#include "CastCommand.hpp"

CastCommand::CastCommand(GameModel &gameModel) : gameModel{gameModel} {}

std::unique_ptr<MessageBuilder> CastCommand::execute(const gsl::span<std::string, -1> arguments, const PlayerInfo &player) {
    return DisplayMessageBuilder{"You attempt to cast a spell"}
            .addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}
