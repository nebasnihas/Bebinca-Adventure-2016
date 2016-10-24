#ifndef COMBATMANAGER_HPP
#define COMBATMANAGER_HPP

#include <string>
#include <algorithm>
#include "CombatInstance.hpp"

class CombatManager {

public:

    void update();
    void setAction(std::string actionID, std::string characterID);


private:

    std::vector<CombatInstance> combatInstances;

};

#endif