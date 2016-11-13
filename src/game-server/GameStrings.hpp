#ifndef ADVENTURE2016_GAMESTRINGS_H
#define ADVENTURE2016_GAMESTRINGS_H


#include <string>
#include "StringResourceImporter.hpp"

namespace GameStringKeys {
    const std::string INVALID_DIR = "invalid-direction";
    const std::string INVALID_TGT = "invalid-target";

    const std::string PLAYERS_AREA = "players-list-area";
    const std::string PLAYERS_WORLD = "players-list-world";
    const std::string EXITS_AREA = "exits-list";

    const std::string UNSPECIFIED_PLAYER = "unspecified-player";
    const std::string UNSPECIFIED_TARGET = "unspecified-target";

    const std::string PLAYER = "player";
    const std::string OBJECT = "object";

    const std::string GLOBAL_CHANNEL = "global-tag";
    const std::string AREA_CHANNEL = "area-tag";
    const std::string PRIVATE_CHANNEL = "private-tag";


	const std::string COMBAT_ENGAGE = "combat-engage-issue";
	const std::string COMBAT_ENGAGED = "combat-engage-receive";
	const std::string PLAYER_ATTACKS = "combat-attack-issue";
	const std::string PLAYER_ATTACKED = "combat-attack-receive";
	const std::string ATTACK_ACTION_ID = "combat-attack-id";
	const std::string COMBAT_VICTORY = "combat-result-victory";
	const std::string COMBAT_LOSS = "combat-result-loss";

	const std::string SPELL_LIST = "spell-list-header";
	const std::string SPELL_NO_MANA = "spell-insufficient-mana";
	const std::string SPELL_UNKNOWN = "spell-unknown";
	const std::string SPELL_OFFENSE_SELF = "spell-offense-self";
	const std::string SPELL_OFFENSE_SOURCE = "spell-offense-source";
	const std::string SPELL_OFFENSE_TARGET = "spell-offense-target";
	const std::string SPELL_DEFENSE_SELF = "spell-defense-self";
	const std::string SPELL_DEFENSE_SOURCE = "spell-defense-source";
	const std::string SPELL_DEFENSE_TARGET = "spell-defense-target";
	const std::string SPELL_GENERIC_SELF = "spell-generic-self";
	const std::string SPELL_GENERIC_SOURCE = "spell-generic-source";
	const std::string SPELL_GENERIC_TARGET = "spell-generic-target";

	const std::string STATUS_EFFECT_END = "status-effect-expiry";
	const std::string CHAR_STATUS = "character-status";
}

class GameStrings {
public:
    static std::string get(const std::string& key) {
        return StringResourceImporter::getInstance().getString(key);
    }

};


#endif //ADVENTURE2016_GAMESTRINGS_H
