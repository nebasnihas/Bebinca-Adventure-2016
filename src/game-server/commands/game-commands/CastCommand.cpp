
#include <commands/DisplayMessageBuilder.hpp>
#include <boost/algorithm/string/join.hpp>
#include "CastCommand.hpp"

CastCommand::CastCommand(GameModel &gameModel, Controller& controller) : gameModel{gameModel}, controller{controller} {}

std::unique_ptr<MessageBuilder> CastCommand::execute(const gsl::span<std::string, -1> arguments, const PlayerInfo &player) {
	if (arguments.empty()) {
		gameModel.listValidSpells(player.playerID);
		return buildPlayerMessage(player.clientID, "Cast what?");
	}
	else if (arguments.size() == 1) {
		return buildPlayerMessage(player.clientID, "Invalid usage of cast. Type \"help cast\"");
	}

	auto spellStrings = arguments.subspan(0, arguments.size() - 1);
	std::string spell = boost::algorithm::join(spellStrings, " ");
	std::string targetID = *(arguments.end() - 1);

	if (!controller.getClientID(targetID)) {
		if (targetID == TARGET_SELF) {
			targetID = player.playerID;
		}
		else {
			return buildPlayerMessage(player.clientID, targetID + " not found");
		}
	}

	gameModel.castSpell(player.playerID, targetID, spell);
	return buildPlayerMessage(player.clientID, "Casting spell " + spell);

}
