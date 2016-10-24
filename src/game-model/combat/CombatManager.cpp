#include "CombatManager.hpp"

void CombatManager::setAction(std::string actionID, std::string characterID) {

}

void CombatManager::update() {
    for (CombatInstance& instance : combatInstances) {
        instance.update();

        if (instance.isBattleOver()) {
            instance.battleCleanup();
        }
    }

    auto eraseIter = std::remove_if(combatInstances.begin(), combatInstances.end(),
                   [] (const CombatInstance& ci) {
                       return ci.isBattleOver();
                   }
    );
    combatInstances.erase(eraseIter, combatInstances.end());
}