#include "SayCommand.hpp"
#include "commands/DisplayMessageBuilder.hpp"
#include <boost/algorithm/string.hpp>
#include <GameStrings.hpp>

SayCommand::SayCommand(GameModel& gameModel, Controller& controller) : gameModel{gameModel}, controller{controller} {}

std::unique_ptr<MessageBuilder> SayCommand::execute(const gsl::span<std::string, -1> arguments,
                                                    const PlayerInfo& player) {
    std::string message = GameStrings::get(GameStringKeys::GLOBAL_CHANNEL) + " " + boost::join(arguments, " ");

    auto clientList = controller.getAllClients();
	for (auto& client: clientList) {
		auto playerID = controller.getPlayerID(client);
		gameModel.getCharacterByID(playerID)->pushToBuffer(message);
	}
	return DisplayMessageBuilder{message};
}

