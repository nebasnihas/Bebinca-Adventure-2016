#ifndef COMBATACTION_HPP
#define COMBATACTION_HPP

#include "CharacterInstance.hpp"

class CombatAction {

public:

    //TODO: Add constructor for new actions with different effects

    void execute(CharacterInstance& source, CharacterInstance& target);

private:

    //TODO: Spell costs/resources/effects

};


#endif