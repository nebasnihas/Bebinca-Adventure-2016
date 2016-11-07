#include "CombatAction.hpp"

CombatCast::CombatCast(const Spell &spell) : spell(spell)
{

}

void CombatAction::dealDamage(CharacterInstance &target, int amount) {
    Character& character = target.getCharacterRef();
    auto newHealth = character.getCurrentHealth() - amount;
    if (newHealth < 0) {
        newHealth = 0;
    }
    character.setCurrentHealth(newHealth);
}

void CombatAction::healDamage(CharacterInstance &target, int amount) {
    auto character = target.getCharacterRef();
    auto maxHealth = character.getMaxHealth();
    auto newHealth = character.getCurrentHealth() + amount;
    if (newHealth > maxHealth) {
        newHealth = maxHealth;
    }
    character.setCurrentHealth(newHealth);
}

void CombatAction::execute(CharacterInstance &source, CharacterInstance &target) {}

std::string CombatAction::getID() {
    return "NULL";
}

void CombatAttack::execute(CharacterInstance &source, CharacterInstance &target) {
    auto sourceLevel = source.getCharacterRef().getLevel();
    // TODO: Move this formula somewhere configurable
    auto damage = 10 * sourceLevel;
    dealDamage(target, damage);

	auto sourceMessage = "You attack " + target.getCharacterRef().getName() + " and deal " + std::to_string(damage) + " damage";
	auto targetMessage = "You take " + std::to_string(damage) + " damage" + " from " + source.getCharacterRef().getName();
	source.getCharacterRef().pushToBuffer(sourceMessage);
	target.getCharacterRef().pushToBuffer(targetMessage);
}

std::string CombatAttack::getID() {
    return "attack";
}

void CombatCast::execute(CharacterInstance &source, CharacterInstance &target) {

    auto manaCost = spell.getManaCost();
    auto character = source.getCharacterRef();
    auto currentMana = character.getCurrentMana();

    if (currentMana < manaCost) {
		source.getCharacterRef().pushToBuffer("Not enough mana to cast " + spell.getName());
        return;
    } else {
        character.setCurrentMana(currentMana - manaCost);
    }

    auto power = spell.getPower(source.getCharacterRef());
    switch (spell.getType()) {
        case SpellType::OFFENSE:
			source.getCharacterRef().pushToBuffer("You cast " + spell.getName() + " for damage " + std::to_string(power));
			target.getCharacterRef().pushToBuffer(source.getCharacterRef().getName() + "casts " + spell.getName() + " on you for damage " + std::to_string(power));
            dealDamage(target, power);
            break;
        case SpellType::DEFENSE:
			source.getCharacterRef().pushToBuffer("You cast " + spell.getName() + " for effect " + std::to_string(power));
			target.getCharacterRef().pushToBuffer(source.getCharacterRef().getName() + "casts " + spell.getName() + " on you for effect " + std::to_string(power));
            healDamage(target, power);
            break;
    }
}

const Spell& CombatCast::getSpellRef() const {
    return spell;
}

std::string CombatCast::getID() {
    return getSpellRef().getName();
}