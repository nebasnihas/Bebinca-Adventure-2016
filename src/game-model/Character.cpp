#include "Character.hpp"

Character::Character(std::string id, std::string name, std::string areaID)
    : id(id), name(name), areaID(areaID), level(STARTING_LEVEL) {
}

Character::~Character() { }

std::string Character::getID() const {
    return id;
}

std::string Character::getName() const {
    return name;
}

std::string Character::getAreaID() const {
    return areaID;
}

//Attributes Character::getAttributes() const {
//    return attributes;
//}


int Character::getLevel() const {
    return level;
}

int Character::getExp() const {
    return exp;
}

int Character::getDamage() const {
    return damage;
}

int Character::getArmor() const {
    return armor;
}

void Character::setID(std::string id) {
    this->id = id;
}

void Character::setName(std::string name) {
    this->name = name;
}

void Character::setAreaID(std::string areaID) {
    this->areaID = areaID;
}

void Character::setDamage(int damage) {
    this->damage = damage;
}

void Character::setArmor(int armor) {
    this->armor = armor;
}

void Character::increaseLevel() {
    this->level++;
}

void Character::increaseExp(int expToAdd) {
    this->exp += expToAdd;
}

//////////////////////////// Skills ////////////////////////////

//std::string Character::getSkillsString() const {
//    std::string skillString;
//
//    for (const auto& item : skills) {
//        skillString += item.first + ": " + std::to_string(item.second) + "\n";
//    }
//
//    return skillString;
//}
//
//int Character::getSkill(std::string skill) {
//    return skills[skill];
//}
//
//void Character::increaseSkill(std::string skill) {
//    skills[skill]++;
//}
