#include "CombatInstance.hpp"

CombatInstance::CombatInstance(std::unordered_map<std::string, CombatAction>& actionLookup) : actionLookup(&actionLookup)
{

}

void CombatInstance::update() {

    //TODO: Order of execution for character actions
    for (CharacterInstance& characterInstance : characters) {

        //TODO: Switch to lookup from provided file
        auto iter = (*actionLookup).find(characterInstance.getCombatActionID());
        if (iter != (*actionLookup).end()) {
            CombatAction& action = iter->second;
            action.execute(characterInstance, characterInstance.getTarget());
        } else {
            //TODO: Add case to use default action
        }

    }

}

// A battle is over when there less than two teams have an alive member
bool CombatInstance::isBattleOver() const {

    int firstTeamSeen = -1;

    for (const CharacterInstance& characterInstance : characters) {
        if (characterInstance.isAlive()) {
            int teamID = characterInstance.getTeamID();

            if (firstTeamSeen == -1) {
                // If this is the first team seen, set that as the ID
                firstTeamSeen = teamID;
            } else {
                // If not, and this is a different ID, then there are at least two parties left
                if (firstTeamSeen != teamID) {
                    return false;
                }
            }
        }
    }

    return true;
}

void CombatInstance::battleCleanup() {
    //TODO: Distribute exp, and respawn dead characters
}