
#include <commands/DisplayMessageBuilder.hpp>
#include "CastCommand.hpp"

CastCommand::CastCommand(GameModel &gameModel, Controller& controller) : gameModel{gameModel}, controller{controller} {}

std::unique_ptr<MessageBuilder> CastCommand::execute(const gsl::span<std::string, -1> arguments, const PlayerInfo &player) {
	if (arguments.empty()) {
		gameModel.listValidSpells(player.playerID);
		return buildPlayerMessage(player.clientID, "Cast what?");
	}

	auto spell = arguments[0];

	auto targetClient = player.clientID;
	if (arguments.size() > 1) {
		auto targetPlayerID = arguments[1];
		if (!controller.getClientID(targetPlayerID)) {
			return buildPlayerMessage(player.clientID, "Player " + targetPlayerID + "not found");
		}
		targetClient = controller.getClientID(targetPlayerID).get();
	}

	gameModel.setCombatAction(player.playerID, spell);
	return buildPlayerMessage(player.clientID, "You attempt to cast " + spell);

}
