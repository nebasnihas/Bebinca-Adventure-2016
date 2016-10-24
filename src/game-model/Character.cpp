#include "../../include/game/Character.hpp"


Character::Character(const std::string& id, const std::string& name, const std::string& areaID)
    : id(id)
    , name(name)
    , areaID(areaID)
    , level(STARTING_LEVEL) 
    {

        try
        {
            if (id == "" ||
                name == "" ||
                areaID == ""
                )
            {
                throw InvalidCharacterException();
            }
        }
        catch(InvalidCharacterException& e)
        {
            std::cout << e.what() << std::endl;
        }

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

void Character::setLevel(int newLevel) {
    this->level = newLevel;
}

void Character::increaseLevel() {
    this->level++;
}

void Character::increaseExp(int expToAdd) {
    this->exp += expToAdd;
}
