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
    Attributes attributes;
    Inventory inventory;

public:
    const int STARTING_LEVEL = 1;

    Character(std::string id, std::string name, std::string areaID);

    std::string getName() const;
    std::string getID() const;
    std::string getAreaID() const;
    int getLevel() const;
    int getExp() const;
    int getDamage() const;
    int getArmor() const;
    Attributes& getAttributes();
    Inventory& getInventory();

    void setID(const std::string& id);
    void setName(const std::string& name);
    void setAreaID(const std::string& areaID);
    void setDamage(int damage);
    void setArmor(int armor);

    void increaseLevel();
    void increaseExp(int expToAdd);
};

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
