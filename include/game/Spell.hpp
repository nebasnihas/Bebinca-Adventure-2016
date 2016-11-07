#ifndef SPELL_HPP
#define SPELL_HPP

#include <string>
#include <game/Character.hpp>
#include <expr-evaluator/ExprEvaluator.hpp>

enum class SpellType {
    OFFENSE,
    DEFENSE,
    STATUS,
    UNDEFINED
};

class Spell {

public:

    Spell();
    Spell(const std::string& name, int manaCost, SpellType type, const std::string& effect);

    std::string getName() const;
    int getManaCost() const;
    int getPower() const;
    SpellType getType() const;
    int getPower(const Character& caster) const;

private:

    std::string name;
    int manaCost;
    int power;
    std::string effect;
    SpellType type;

};

#endif