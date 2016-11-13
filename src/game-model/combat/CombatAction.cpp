#include <boost/regex/v4/match_flags.hpp>
#include "CombatAction.hpp"
#include "../../game-server/GameStrings.hpp"
#include <boost/format.hpp>

CombatCast::CombatCast(const Spell &spell) : spell(spell)
{

}

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

	auto sourceMessage = (boost::format(GameStrings::get(GameStringKeys::PLAYER_ATTACKS)) % target.getName() % damage).str();
	auto targetMessage = (boost::format(GameStrings::get(GameStringKeys::PLAYER_ATTACKED)) % damage % source.getName()).str();
	source.pushToBuffer(sourceMessage);
	target.pushToBuffer(targetMessage);
}

std::string CombatAttack::getID() {
    return "attack";
}

void CombatCast::execute(Character& source, Character& target) {

    auto manaCost = spell.getManaCost();
    auto currentMana = source.getCurrentMana();

    if (currentMana < manaCost) {
		source.pushToBuffer((boost::format(GameStrings::get(GameStringKeys::SPELL_NO_MANA)) % spell.getName()).str());
        return;
    } else {
        source.setCurrentMana(currentMana - manaCost);
    }


    auto power = spell.getPower(source);
    switch (spell.getType()) {
		case SpellType::OFFENSE:
			if (source.getName() == target.getName()) {
				auto sourceMessage = (boost::format(GameStrings::get(GameStringKeys::SPELL_OFFENSE_SELF)) % spell.getName() % power).str();
				source.pushToBuffer(sourceMessage);
			}
			else {
				auto sourceMessage = (boost::format(GameStrings::get(GameStringKeys::SPELL_OFFENSE_SOURCE))
									  % spell.getName() % target.getName % power).str();
				source.pushToBuffer(sourceMessage);

				auto targetMessage = (boost::format(GameStrings::get(GameStringKeys::SPELL_OFFENSE_TARGET))
									  % source.getName() % spell.getName() % power).str();
				target.pushToBuffer(targetMessage);
			}
            dealDamage(target, power);
            break;
        case SpellType::DEFENSE:
            // TODO: Resolve and fix this temporary patchwork
			if (source.getName() == target.getName() || source.getState() == CharacterState::BATTLE) {
				auto sourceMessage = (boost::format(GameStrings::get(GameStringKeys::SPELL_DEFENSE_SELF)) % spell.getName() % power).str();
				source.pushToBuffer(sourceMessage);
				healDamage(source, power);
			}
			else {
				auto sourceMessage = (boost::format(GameStrings::get(GameStringKeys::SPELL_DEFENSE_SOURCE))
									  % spell.getName() % target.getName % power).str();
				source.pushToBuffer(sourceMessage);

				auto targetMessage = (boost::format(GameStrings::get(GameStringKeys::SPELL_DEFENSE_TARGET))
									  % source.getName() % spell.getName % power).str();
				target.pushToBuffer(targetMessage);

				healDamage(target, power);
			}
            break;
		case SpellType::BODY_SWAP:
			//TODO: determine duration of body swap based on player level
			auto sourceStatus = std::make_shared<BodySwapStatus>(10, target.getID());
			target.pushToBuffer((boost::format(GameStrings::get(GameStringKeys::SPELL_GENERIC_SOURCE))
								 % spell.getName() % target.getName()).str());
			source.addStatusEffect(sourceStatus);

			auto targetStatus = std::make_shared<BodySwapStatus>(10, source.getID());
			source.pushToBuffer((boost::format(GameStrings::get(GameStringKeys::SPELL_GENERIC_SOURCE))
								 % source.getName() % spell.getName()).str());
			target.addStatusEffect(targetStatus);
    }
}

const Spell& CombatCast::getSpellRef() const {
    return spell;
}

std::string CombatCast::getID() {
    return getSpellRef().getName();
}