#include "CombatAction.hpp"

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

	auto sourceMessage = "You attack " + target.getName() + " and deal " + std::to_string(damage) + " damage";
	auto targetMessage = "You take " + std::to_string(damage) + " damage" + " from " + source.getName();
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
		source.pushToBuffer("Not enough mana to cast " + spell.getName());
        return;
    } else {
        source.setCurrentMana(currentMana - manaCost);
    }

    auto power = spell.getPower(source);
    switch (spell.getType()) {
		case SpellType::OFFENSE:
			if (source.getName() == target.getName()) {
				source.pushToBuffer("You cast " + spell.getName() + " on yourself for damage " + std::to_string(power));
			}
			else {
				source.pushToBuffer("You cast " + spell.getName() + " on " + target.getName() + " for damage " + std::to_string(power));
				target.pushToBuffer(source.getName() + " casts " + spell.getName() + " on you for damage " + std::to_string(power));
			}
            dealDamage(target, power);
            break;
        case SpellType::DEFENSE:
            // TODO: Resolve and fix this temporary patchwork
			if (source.getName() == target.getName() || source.getState() == CharacterState::BATTLE) {
				source.pushToBuffer("You cast " + spell.getName() + " on yourself and heal " + std::to_string(power));
				healDamage(source, power);
			}
			else {
				source.pushToBuffer("You cast " + spell.getName() + " on " + target.getName() + " and heal " + std::to_string(power));
				target.pushToBuffer(source.getName() + " casts " + spell.getName() + " on you and heals you for " + std::to_string(power));
				healDamage(target, power);
			}
            break;
    }
}

const Spell& CombatCast::getSpellRef() const {
    return spell;
}

std::string CombatCast::getID() {
    return getSpellRef().getName();
}