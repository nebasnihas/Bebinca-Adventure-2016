#include <commands/DisplayMessageBuilder.hpp>
#include "WhisperCommand.hpp"
#include <boost/algorithm/string.hpp>

WhisperCommand::WhisperCommand(Controller& controller) : controller{controller} {}

std::unique_ptr<MessageBuilder> WhisperCommand::execute(const gsl::span<std::string, -1> arguments,
                                                        const PlayerInfo& player) {
    if (arguments.empty()) {
        //TODO: extract into string resources
        return DisplayMessageBuilder{"Player not specified for whisper"}
                .addClient(player.clientID)
                .setSender(DisplayMessageBuilder::SENDER_SERVER);
    }

    auto targetPlayerID = arguments[0];
    auto targetClient = controller.getClientID(targetPlayerID);

    if (!targetClient) {
        //TODO: extract into string resources
        return DisplayMessageBuilder{targetPlayerID + " " + "is not here"}
                .addClient(player.clientID)
                .setSender(DisplayMessageBuilder::SENDER_SERVER);
    }

    auto message = "(Whisper) " + boost::algorithm::join(arguments.subspan(1, arguments.length() - 1), " ");
    return DisplayMessageBuilder{message}
            .addClient(targetClient.get())
            .setSender(player.playerID);
}
