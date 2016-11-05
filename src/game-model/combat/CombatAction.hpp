#ifndef COMBATACTION_HPP
#define COMBATACTION_HPP

#include <string>
#include "CharacterInstance.hpp"
#include "../Spell.hpp"

class CombatAction {

public:

    //TODO: Add constructor for new actions with different effects
    virtual void execute(CharacterInstance& source, CharacterInstance& target);
    virtual std::string getID();

    static void dealDamage(CharacterInstance& target, int amount);
    static void healDamage(CharacterInstance& target, int amount);

private:

    //TODO: Spell costs/resources/effects

};

class CombatAttack : public CombatAction {

public:

    virtual void execute(CharacterInstance& source, CharacterInstance& target);
    virtual std::string getID();

private:

};

class CombatCast : public CombatAction {

public:

    CombatCast(const Spell& spell);
    virtual void execute(CharacterInstance& source, CharacterInstance& target);
    virtual std::string getID();
    const Spell& getSpellRef() const;

private:

    const Spell& spell;

};

#endif