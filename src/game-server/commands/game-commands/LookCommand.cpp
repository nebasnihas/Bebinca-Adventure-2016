#include <commands/DisplayMessageBuilder.hpp>
#include <boost/algorithm/string/join.hpp>
#include "LookCommand.hpp"

LookCommand::LookCommand(GameModel& gameModel) : gameModel{gameModel} {}

std::unique_ptr<MessageBuilder> LookCommand::execute(const gsl::span<std::string, -1> arguments,
													 const PlayerInfo& player) {
	auto areaID = gameModel.getCharacterByID(player.playerID)->getAreaID();
	std::string description;

	if (arguments.empty()) {
		description = gameModel.getAreaByID(areaID)->getTitle() + "\n" + gameModel.getAreaDescription(areaID);
	} else {
		auto targetEntity = arguments[0];
		auto npc = gameModel.getNPCInArea(targetEntity, areaID);
		if (npc == nullptr) {
			description = targetEntity + " " + GameStrings::get(GameStringKeys::INVALID_TGT);
		}
		else {
			description = npc->getlongDesc();
		}
	}

	gameModel.getCharacterByID(player.playerID)->pushToBuffer(description, GameStringKeys::MESSAGE_SENDER_SERVER, ColorTag::WHITE);
	return DisplayMessageBuilder{description};
}
