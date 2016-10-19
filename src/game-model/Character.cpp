#include "Character.hpp"

Character::Character(std::string id, std::string name, std::string areaID)
    : id(id), name(name), areaID(areaID), level(STARTING_LEVEL) {
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

Attributes& Character::getAttributes() {
    return attributes;
}

Inventory& Character::getInventory() {
    return inventory;
}

void Character::setID(const std::string& id) {
    this->id = id;
}

void Character::setName(const std::string& name) {
    this->name = name;
}

void Character::setAreaID(const std::string& areaID) {
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
