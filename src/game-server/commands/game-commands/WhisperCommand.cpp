#include <commands/DisplayMessageBuilder.hpp>
#include "WhisperCommand.hpp"
#include <boost/algorithm/string.hpp>
#include "GameStrings.hpp"

WhisperCommand::WhisperCommand(GameModel& gameModel, Controller& controller) : gameModel{gameModel}, controller{controller} {}

std::unique_ptr<MessageBuilder> WhisperCommand::execute(const gsl::span<std::string, -1> arguments,
                                                        const PlayerInfo& player) {
    if (arguments.empty()) {
        return DisplayMessageBuilder{GameStrings::get(GameStringKeys::UNSPECIFIED_PLAYER)}
                .addClient(player.clientID)
                .setSender(DisplayMessageBuilder::SENDER_SERVER);
    }

    auto targetPlayerID = arguments[0];
    auto targetClient = controller.getClientID(targetPlayerID);

    if (!targetClient) {
        return DisplayMessageBuilder{GameStrings::get(GameStringKeys::PLAYER)
                                     + " " + targetPlayerID + " "
                                     + GameStrings::get(GameStringKeys::INVALID_TGT)}
                .addClient(player.clientID)
                .setSender(DisplayMessageBuilder::SENDER_SERVER);
    }

    auto message = GameStrings::get(GameStringKeys::PRIVATE_CHANNEL) + " "
                   + boost::algorithm::join(arguments.subspan(1, arguments.length() - 1), " ");

	gameModel.getCharacterByID(targetPlayerID)->pushToBuffer(message, player.playerID, ColorTag::WHITE);
    return DisplayMessageBuilder{message};
}
