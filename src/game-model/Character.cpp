#include "Character.hpp"

Character::Character(const std::string& id,
                     const std::string& name, //Shortdesc is a name
                     const std::string& hit,
                     const std::string damage,
                     int level,
                     int exp,
                     int armor,
                     int gold
                     //Inventory inventory
                    )

                    : id(id)
                    , name(name)
                    , hit(hit)
                    , damage(damage)
                    , level(level)
                    , exp(exp)
                    , armor(armor)
                    , gold(gold)
                    //, Inventory(inventory)

                    {
                    }

//Getters
std::string Character::getID() const {
    return id;
}

std::string Character::getName() const {
    return name;
}

std::string Character::getHit() const {
    return hit;
}

std::string Character::getDamage() const {
    return damage;
}

int Character::getLevel() const {
    return level;
}

int Character::getExp() const {
    return exp;
}


int Character::getArmor() const {
    return armor;
}

int Character::getGold() const{
    return gold;
}

//Inventory& Character::getInventory() {
//    return inventory;
//}

//Setters

void Character::setID(const std::string& id) {
    this->id = id;
}

void Character::setName(const std::string& name) {
    this->name = name;
}

void Character::setDamage(const std::string& damage) {
    this->damage = damage;
}

void Character::setArmor(int armor) {
    this->armor = armor;
}

void Character::setGold(const int gold) {
    this->gold = gold;
}

void Character::setLevel(int newLevel) {
    this->level = newLevel;
}

//void Character::setInventory(const std::string& objectID) {
//    this->inventory = inventory.pushback(objectID);
//}

void Character::increaseLevel() {
    this->level++;
}

void Character::increaseExp(int expToAdd) {
    this->exp += expToAdd;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////                                     NPC Subclass                                               ///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

NPC::NPC(const std::string& id,
                   const std::string& name,
                   const std::string& hit,
                   const std::string damage,
                   int level,
                   int exp,
                   int armor,
                   const int thac0,
                   const int gold,
                   const std::string& description,
                   const std::string& keywords,
                   const std::string& longdesc
                   //Inventory inventory
                    )

                    : Character(id,
                        name, //Shortdesc is a name
                        hit,
                        damage,
                        level,
                        exp,
                        armor,
                        gold
                        //inventory
                        )

                    , thac0(thac0)
                    , description(description)
                    , keywords(keywords)
                    , longDesc(longDesc)
                    {
                    }


//Getters
std::string NPC::getDescription() const {
    return description;
}

std::string NPC::getKeywords() const {
    return keywords;
}

std::string NPC::getlongDesc() const {
    return longDesc;
}

int NPC::getThac0() const {
    return thac0;
}
