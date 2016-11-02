#ifndef COMBATINSTANCE_HPP
#define COMBATINSTANCE_HPP

#include <vector>
#include <unordered_map>
#include <algorithm>
#include "CharacterInstance.hpp"
#include "CombatAction.hpp"

class CombatInstance {

public:

    CombatInstance(std::unordered_map<std::string, CombatAction>* actionLookup, std::string defaultActionID);

    CharacterInstance* getCharacterInstance(const std::string& characterID);

    void update();

    bool isBattleOver() const;
    void battleCleanup();

    int addCharacterToNewTeam(Character& character);
    int addCharacterToTeam(Character& character, int teamID);
    bool setupInstance();

    static bool isAvailable(const CharacterInstance& character);

private:

    int getOpenTeamID() const;
    std::unordered_map<int, CharacterInstance*> getDefaultTargets();

    std::string defaultActionID;
    std::unordered_map<std::string, CombatAction>* actionLookup;
    std::vector<CharacterInstance> characters;

};


#endif