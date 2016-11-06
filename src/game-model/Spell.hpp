#ifndef SPELL_HPP
#define SPELL_HPP

#include <string>
#include <game/Character.hpp>

enum SpellType {
    OFFENSE,
    DEFENSE,
    STATUS
};

class Spell {

public:

    Spell(const std::string& name, int manaCost, int power, SpellType type, int (*fPtr)(int));

    std::string getName() const;
    int getManaCost() const;
    int getPower() const;
    SpellType getType() const;
    int getPower(const Character& caster) const;

private:

    std::string name;
    int manaCost;
    int power;

    int (*fPtr)(int);
    SpellType type;

};

#endif