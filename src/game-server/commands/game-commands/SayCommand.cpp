#include "SayCommand.hpp"
#include "commands/DisplayMessageBuilder.hpp"
#include <boost/algorithm/string.hpp>
#include <GameStrings.hpp>

SayCommand::SayCommand(Controller& controller) : controller{controller} {}

std::unique_ptr<MessageBuilder> SayCommand::execute(const gsl::span<std::string, -1> arguments,
                                                    const PlayerInfo& player) {
    std::string message = GameStrings::get(GameStringKeys::GLOBAL_CHANNEL) + " " + boost::join(arguments, " ");

    auto clientList = controller.getAllClients();

    return DisplayMessageBuilder{message}
            .addClients(clientList)
            .setSender(player.playerID);
}

