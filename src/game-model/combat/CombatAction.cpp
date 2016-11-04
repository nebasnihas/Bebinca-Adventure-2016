#include "CombatAction.hpp"

CombatCast::CombatCast(const Spell &spell) : spell(spell)
{

}

void CombatAction::dealDamage(CharacterInstance &target, int amount) {
    auto character = target.getCharacterRef();
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
    return "";
}

void CombatAttack::execute(CharacterInstance &source, CharacterInstance &target) {
    auto sourceLevel = source.getCharacterRef().getLevel();
    // TODO: Move this formula somewhere configurable
    auto damage = 10 * sourceLevel;
    dealDamage(target, damage);
}

std::string CombatAttack::getID() {
    return "attack";
}

void CombatCast::execute(CharacterInstance &source, CharacterInstance &target) {

    auto manaCost = spell.getManaCost();
    auto character = source.getCharacterRef();
    auto currentMana = character.getCurrentMana();

    if (currentMana < manaCost) {
        // Not enough mana for spell cast
        // TODO: Send error message to client
        return;
    } else {
        character.setCurrentMana(currentMana - manaCost);
    }

    auto power = spell.getPower(source.getCharacterRef());
    switch (spell.getType()) {
        case SpellType::OFFENSE:
            dealDamage(target, power);
            break;
        case SpellType::DEFENSE:
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