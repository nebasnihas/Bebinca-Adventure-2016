#ifndef CHARACTER_H
#define CHARACTER_H

#include "Attributes.hpp"
#include "Inventory.hpp"

class Character {
protected:
    std::string id;
    std::string name;
    std::string areaID;
    int level;
    int exp;
    int damage;
    int armor;
//    std::unordered_map<std::string, int> skills;

public:
    const int STARTING_LEVEL = 1;

    Attributes attributes;
    Inventory inventory;

    Character(std::string id, std::string name, std::string areaID);
    ~Character();

    std::string getName() const;
    std::string getID() const;
    std::string getAreaID() const;
//    Attributes getAttributes() const;
    std::string getClass() const;
    int getLevel() const;
    int getExp() const;
    int getDamage() const;
    int getArmor() const;

    void setID(std::string id);
    void setName(std::string name);
    void setAreaID(std::string areaID);
    void setDamage(int damage);
    void setArmor(int armor);

    void increaseLevel();
    void increaseExp(int expToAdd);

    //////////////////////////// Skills ////////////////////////////
//    std::string getSkillsString() const;
//    int getSkill(std::string skill);
//    void increaseSkill(std::string skill);
};

// TODO move derived Character class implementations to separate .cpp files

class WarriorCharacter : public Character {
public:
    WarriorCharacter(std::string id, std::string name, std::string areaID)
    : Character(id, name, areaID) {
        attributes.setPrimaryAttribute("strength");
    }

    std::string getClass() {
        return "warrior";
    }
};

class WizardCharacter : public Character {
public:
    WizardCharacter(std::string id, std::string name, std::string areaID)
            : Character(id, name, areaID) {
        attributes.setPrimaryAttribute("intelligence");
    }

    std::string getClass() {
        return "wizard";
    }
};

class RangerCharacter : public Character {
public:
    RangerCharacter(std::string id, std::string name, std::string areaID)
            : Character(id, name, areaID) {
        attributes.setPrimaryAttribute("dexterity");
    }

    std::string getClass() {
        return "ranger";
    }
};

#endif
