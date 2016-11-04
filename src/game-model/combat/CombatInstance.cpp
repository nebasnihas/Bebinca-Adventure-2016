#include "CombatInstance.hpp"

CombatInstance::CombatInstance(std::unordered_map<std::string, CombatAction>* actionLookup, std::string defaultActionID)
        : actionLookup(actionLookup), defaultActionID(defaultActionID)
{

}

void CombatInstance::update() {

    //TODO: Order of execution for character actions
    for (CharacterInstance& characterInstance : characters) {

        if (!characterInstance.isAlive()) {
            continue;
        }

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
    for (const auto& cInstance : characters) {

        auto character = cInstance.getCharacterRef();
        if (cInstance.isAlive()) {
            // Character is alive, distribute exp and continue
            // TODO: Add actual exp tracking
            character.increaseLevel();
            character.setState(CharacterState::IDLE);
        } else {
            // Character is dead. Set to dead state, GameModel will clean up later.
            character.setState(CharacterState::DEAD);
        }
    }
}

// TODO: Makes assumptions that teamIDs are added incrementally
int CombatInstance::getOpenTeamID() const {
    auto maxTeamID = std::max_element(characters.begin(), characters.end(),
        [] (const auto& a, const auto& b) { return a.getTeamID() < b.getTeamID(); })->getTeamID();
    return maxTeamID + 1;
}

int CombatInstance::addCharacterToNewTeam(Character& character) {
    return addCharacterToTeam(character, getOpenTeamID());
}

int CombatInstance::addCharacterToTeam(Character& character, int teamID) {
    CharacterInstance cInstance(character, defaultActionID, CharacterInstance::nullTarget, teamID);
    characters.push_back(cInstance);
    return teamID;
}

bool CombatInstance::setupInstance() {

    // Initial checks
    if (characters.capacity() <= 1) {
        return false;
    }

    bool bAllAvailable = std::all_of(characters.begin(), characters.end(),
        [] (const auto& c) { return CombatInstance::isAvailable(c); });

    // One of the characters is already in combat, cancel out
    if (!bAllAvailable) {
        return false;
    }

    for (const auto& character : characters) {
        character.getCharacterRef().setState(CharacterState::BATTLE);
    }

    // Make sure all characters have a default target
    auto defaultTargets = getDefaultTargets();
    std::vector<int> mapKeys;
    for (const auto& pair : defaultTargets) {
        mapKeys.push_back(pair.first);
    }

    for (auto& character : characters) {
        if (&character.getTarget() == &CharacterInstance::nullTarget) {
            int index = 0;
            while (character.getTeamID() == index) {
                index++;
            }
            character.setTarget(*defaultTargets[index]);
        }
    }

    return true;
}

bool CombatInstance::isAvailable(const CharacterInstance& character) {
    return character.getCharacterRef().getState() == CharacterState::IDLE;
}

std::unordered_map<int, CharacterInstance*> CombatInstance::getDefaultTargets() {
    std::unordered_map<int, CharacterInstance*> defaultTargets;
    for (auto& character : characters) {
        auto teamID = character.getTeamID();
        if (defaultTargets.count(teamID) == 0) {
            defaultTargets[teamID] = &character;
        }
    }
    return defaultTargets;
}

CharacterInstance* CombatInstance::getCharacterInstance(const std::string& characterID) {
    for (auto& characterInstance : characters) {
        if (characterInstance.getCharacterRef().getID() == characterID) {
            return &characterInstance;
        }
    }
    return nullptr;
}

std::vector<std::string> CombatInstance::getPossibleTargets(const std::string& characterID) {
    std::vector<std::string> possibleTargets;
    auto character = getCharacterInstance(characterID);

    if (character == nullptr) {
        return possibleTargets;
    }

    auto teamID = character->getTeamID();
    for (const auto& ci : characters) {
        if (ci.getTeamID() == teamID) {
            continue;
        }
        auto ciID = ci.getCharacterRef().getID();
        if (ciID != characterID) {
            possibleTargets.push_back(ciID);
        }
    }
    return possibleTargets;
}