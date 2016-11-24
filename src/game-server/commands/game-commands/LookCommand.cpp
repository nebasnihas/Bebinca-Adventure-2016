#include <commands/DisplayMessageBuilder.hpp>
#include <boost/algorithm/string/join.hpp>
#include "LookCommand.hpp"

LookCommand::LookCommand(GameModel& gameModel) : gameModel{gameModel} {}

std::unique_ptr<MessageBuilder> LookCommand::execute(const gsl::span<std::string, -1> arguments,
													 const PlayerInfo& player) {
	auto areaID = gameModel.getCharacterByID(player.playerID)->getAreaID();
	std::string description;

	if (arguments.empty()) {
		description = gameModel.getAreaDescription(areaID);
	} else {
		auto targetEntity = boost::algorithm::join(arguments, " ");
		auto NPCIDs = gameModel.getNPCIDsInArea(areaID);
		if (std::find(NPCIDs.begin(), NPCIDs.end(), targetEntity) != NPCIDs.end()) {
			description = gameModel.getNPCByID(targetEntity)->getlongDesc();
		}
		else {
			description = gameModel.getEntityDescription(areaID, targetEntity);
		}
	}

	gameModel.getCharacterByID(player.playerID)->pushToBuffer(description, GameStringKeys::MESSAGE_SENDER_SERVER, ColorTag::WHITE);
	return DisplayMessageBuilder{description};
}
