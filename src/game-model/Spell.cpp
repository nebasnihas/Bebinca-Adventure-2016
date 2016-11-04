#include "Spell.hpp"

Spell::Spell(const std::string& name, int manaCost, int power, SpellType type, int (*fPtr)(int))
        : name(name), manaCost(manaCost), power(power), type(type), fPtr(fPtr)
{

}

std::string Spell::getName() const {
    return this->name;
}

int Spell::getPower() const {
    return this->power;
}

int Spell::getManaCost() const {
    return this->manaCost;
}

SpellType Spell::getType() const {
    return this->type;
}

int Spell::getPower(const Character &caster) const {
    return (fPtr)(caster.getLevel());
}