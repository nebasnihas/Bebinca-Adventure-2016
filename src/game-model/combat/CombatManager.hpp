#ifndef COMBATMANAGER_HPP
#define COMBATMANAGER_HPP

#include <string>
#include "CombatInstance.hpp"

class CombatManager {

public:

    void update();
    void setActionLookup(const std::unordered_map<std::string, CombatAction> actionLookup);
    void beginBattle(const std::vector<Character&>& characters);
    void setAction(std::string actionID, std::string characterID);
    CombatInstance& getNewCombatInstance();

    bool loadCombatInstance(CombatInstance& combatInstance);
    CombatInstance* getCombatInstanceByCharacterID(const std::string& characterID);
    CharacterInstance* getCharacterInstanceByCharacterID(const std::string& characterID);

private:

    std::string defaultActionID;
    std::unordered_map<std::string, CombatAction> actionLookup;
    std::vector<CombatInstance> combatInstances;

};

#endif