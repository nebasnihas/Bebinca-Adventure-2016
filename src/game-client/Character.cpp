#include "Character.h"

Character::Character(string name, Type type) : name(name), type(type), level(1) {
    // TODO determine unique stats and attacks for character type
    // TODO initialize skills
}

string Character::getName() const {
    return name;
}

int Character::getAreaID() const {
    return areaID;
}

Character::Type Character::getType() const {
    return type;
}

void Character::setAreaID(int areaID) {
    this->areaID = areaID;
}

//////////////////////////// Level ////////////////////////////

void Character::increaseLevel() {
    level++;
}

int Character::getLevel() const {
    return level;
}

//////////////////////////// Stats ////////////////////////////

unordered_map<string, int> Character::getStats() const {
    return stats;
}

int Character::getStat(string stat) {
    return stats[stat];
}

void Character::increaseStat(string stat) {
    stats[stat]++;
}

//////////////////////////// Skills ////////////////////////////

string Character::getSkillsString() const {
    string skillString;

    for (const auto& item : skills) {
        skillString += item.first + ": " + to_string(item.second) + "\n";
    }

    return skillString;
}

int Character::getSkill(string skill) {
    return skills[skill];
}

void Character::increaseSkill(string skill) {
    skills[skill]++;
}
