#include "game/Spell.hpp"

Spell::Spell() : Spell("", 0, SpellType::UNDEFINED, "")
{

}

Spell::Spell(const std::string& name, int manaCost, SpellType type, const std::string& effect)
        : name(name), manaCost(manaCost), type(type), effect(effect)
{

}

std::string Spell::getName() const {
    return this->name;
}

int Spell::getManaCost() const {
    return this->manaCost;
}

SpellType Spell::getType() const {
    return this->type;
}

int Spell::getPower(const Character &caster) const {
    return std::stoi(ExprEvaluator::evaluate_infix_1(effect, caster.getLevel()));
}