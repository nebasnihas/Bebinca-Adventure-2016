#include "CharacterInstance.hpp"

CharacterInstance::CharacterInstance(Character& character, const std::string& initialAction, CharacterInstance& target)
        : character(character), selectedActionID(initialAction), target(&target)
{

};

std::string CharacterInstance::getCombatActionID() const {
    return this->selectedActionID;
}

void CharacterInstance::setCombatActionID(const std::string &combatAction) {
    this->selectedActionID = combatAction;
}

CharacterInstance& CharacterInstance::getTarget() {
    return *(this->target);
}

void CharacterInstance::setTarget(CharacterInstance &target) {
    this->target = &target;
}

bool CharacterInstance::isAlive() const {
    //TODO return current health != 0
    return true;
}

int CharacterInstance::getTeamID() const {
    return this->teamID;
}

void CharacterInstance::setTeamID(const int teamID) {
    this->teamID = teamID;
}