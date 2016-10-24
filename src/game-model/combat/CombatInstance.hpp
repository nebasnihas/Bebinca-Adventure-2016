#ifndef COMBATINSTANCE_HPP
#define COMBATINSTANCE_HPP

#include <vector>
#include <unordered_map>
#include "CharacterInstance.hpp"
#include "CombatAction.hpp"

class CombatInstance {

public:

    CombatInstance(std::unordered_map<std::string, CombatAction>& actionLookup);

    void update();

    bool isBattleOver() const;
    void battleCleanup();

private:

    std::unordered_map<std::string, CombatAction>* actionLookup;
    std::vector<CharacterInstance> characters;

};


#endif