#include <boost/regex/v4/match_flags.hpp>
#include "CombatAction.hpp"
#include <boost/format.hpp>

CombatCast::CombatCast(const Spell &spell) : spell(spell) {}

void CombatAction::dealDamage(Character& character, int amount) {
    auto newHealth = character.getCurrentHealth() - amount;
    if (newHealth < 0) {
        newHealth = 0;
    }
    character.setCurrentHealth(newHealth);
}

void CombatAction::healDamage(Character& character, int amount) {
    auto maxHealth = character.getMaxHealth();
    auto newHealth = character.getCurrentHealth() + amount;
    if (newHealth > maxHealth) {
        newHealth = maxHealth;
    }
    character.setCurrentHealth(newHealth);
}

void CombatAction::execute(Character& source, Character& target) {}

std::string CombatAction::getID() {
    return "NULL";
}

void CombatAttack::execute(Character& source, Character& target) {
    auto sourceLevel = source.getLevel();
    // TODO: Move this formula somewhere configurable
    auto damage = 10 * sourceLevel;
    dealDamage(target, damage);
	auto stringInfo = StringInfo{source.getName(), target.getName(), damage, getID()};

	auto sourceMessage = GameStrings::getFormatted(GameStringKeys::PLAYER_ATTACKS, stringInfo);
	auto targetMessage = GameStrings::getFormatted(GameStringKeys::PLAYER_ATTACKED, stringInfo);
	source.pushToBuffer(sourceMessage, GameStringKeys::MESSAGE_SENDER_BATTLE, 0);
	target.pushToBuffer(targetMessage, GameStringKeys::MESSAGE_SENDER_BATTLE, 0);
}

std::string CombatAttack::getID() {
    return "attack";
}

void CombatCast::execute(Character& source, Character& target) {
    auto manaCost = spell.getManaCost();
    auto currentMana = source.getCurrentMana();
	auto power = spell.getPower(source);
	auto stringInfo = StringInfo{source.getName(), target.getName(), power, spell.getName()};
    if (currentMana < manaCost) {
		source.pushToBuffer(GameStrings::getFormatted(GameStringKeys::SPELL_NO_MANA, stringInfo), GameStringKeys::MESSAGE_SENDER_SERVER, 0);
        return;
    } else {
        source.setCurrentMana(currentMana - manaCost);
    }

    switch (spell.getType()) {
		case SpellType::OFFENSE:
			castOffenseSpell(source, target, power, stringInfo);
            break;
        case SpellType::DEFENSE:
			castDefenseSpell(source, target, power, stringInfo);
			break;
		case SpellType::BODY_SWAP:
			//TODO: determine duration of body swap based on player level
			auto sourceStatus = std::make_shared<BodySwapStatus>(10, target.getID());
			target.pushToBuffer(GameStrings::getFormatted(GameStringKeys::SPELL_GENERIC_SOURCE, stringInfo), GameStringKeys::MESSAGE_SENDER_SERVER, 0);
			source.addStatusEffect(sourceStatus);

			auto targetStatus = std::make_shared<BodySwapStatus>(10, source.getID());
			source.pushToBuffer(GameStrings::getFormatted(GameStringKeys::SPELL_GENERIC_TARGET, stringInfo), GameStringKeys::MESSAGE_SENDER_SERVER, 0);
			target.addStatusEffect(targetStatus);
    }
}

void CombatCast::castDefenseSpell(Character &source, Character &target, int power, const StringInfo &stringInfo) const {
	auto senderID = GameStringKeys::MESSAGE_SENDER_SERVER;
	if (source.getState() == BATTLE) {
		senderID = GameStringKeys::MESSAGE_SENDER_BATTLE;
	}
	if (source.getName() == target.getName() || source.getState() == BATTLE) {
		source.pushToBuffer(GameStrings::format(spell.getHitvict(), stringInfo), senderID, 0);
		healDamage(source, power);
	}
	else {
		source.pushToBuffer(GameStrings::format(spell.getHitchar(), stringInfo), senderID, 0);
		target.pushToBuffer(GameStrings::format(spell.getHitvict(), stringInfo), senderID, 0);
		healDamage(target, power);
	}
}

void CombatCast::castOffenseSpell(Character &source, Character &target, int power, const StringInfo &stringInfo) const {
	if (source.getName() == target.getName()) {
		source.pushToBuffer(GameStrings::format(spell.getHitvict(), stringInfo), GameStringKeys::MESSAGE_SENDER_SERVER, 0);
	}
	else {
		source.pushToBuffer(GameStrings::format(spell.getHitchar(), stringInfo), GameStringKeys::MESSAGE_SENDER_BATTLE, 0);
		target.pushToBuffer(GameStrings::format(spell.getHitvict(), stringInfo), GameStringKeys::MESSAGE_SENDER_BATTLE, 0);
	}
	dealDamage(target, power);
}


const Spell& CombatCast::getSpellRef() const {
    return spell;
}

std::string CombatCast::getID() {
    return getSpellRef().getName();
}