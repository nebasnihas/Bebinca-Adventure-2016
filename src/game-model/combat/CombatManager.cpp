#include "CombatManager.hpp"

void CombatManager::setAction(std::string actionID, std::string characterID) {

}

void CombatManager::setActionLookup(const std::unordered_map<std::string, CombatAction> actionLookup) {
    this->actionLookup = std::move(actionLookup);
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

CombatInstance& CombatManager::getNewCombatInstance() {
    CombatInstance cInstance(&actionLookup, defaultActionID);
}

bool CombatManager::loadCombatInstance(CombatInstance& combatInstance) {
    bool bSetupSuccess = combatInstance.setupInstance();
    if (!bSetupSuccess) {
        return false;
    }
    combatInstances.push_back(std::move(combatInstance));
    return true;
}

CombatInstance* CombatManager::getCombatInstanceByCharacterID(const std::string& characterID) {
    for (auto& combatInstance : combatInstances) {
        auto characterInstance = combatInstance.getCharacterInstance(characterID);
        if (characterInstance != nullptr) {
            return &combatInstance;
        }
    }
    return nullptr;
}

CharacterInstance* CombatManager::getCharacterInstanceByCharacterID(const std::string& characterID) {
    for (auto& combatInstance : combatInstances) {
        auto characterInstance = combatInstance.getCharacterInstance(characterID);
        if (characterInstance != nullptr) {
            return characterInstance;
        }
    }
    return nullptr;
}