#include "Character.hpp"

Character::Character(std::string id, std::string areaID, std::string name, Type type, int level) : id(id), areaID(areaID), name(name), type(type), level(level) {
    // TODO determine unique stats and attacks for character type
    // TODO initialize skills
}

std::string Character::getID() const {
    return id;
}

std::string Character::getName() const {
    return name;
}

std::string Character::getAreaID() const {
    return areaID;
}

Character::Type Character::getType() const {
    return type;
}

void Character::setID(std::string id) {
    this->id = id;
}

void Character::setAreaID(std::string areaID) {
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

std::unordered_map<std::string, int> Character::getStats() const {
    return stats;
}

int Character::getStat(std::string stat) {
    return stats[stat];
}

void Character::increaseStat(std::string stat) {
    stats[stat]++;
}

//////////////////////////// Skills ////////////////////////////

std::string Character::getSkillsString() const {
    std::string skillString;

    for (const auto& item : skills) {
        skillString += item.first + ": " + std::to_string(item.second) + "\n";
    }

    return skillString;
}

int Character::getSkill(std::string skill) {
    return skills[skill];
}

void Character::increaseSkill(std::string skill) {
    skills[skill]++;
}
