#ifndef CHARACTER_H
#define CHARACTER_H

#include "Attributes.hpp"
#include "Inventory.hpp"

enum CharacterState {
    IDLE,
    BATTLE,
    DEAD
};

class Character {
protected:
    std::string id;
    std::string name;
    std::string areaID;
    int level;
    int exp;
    int damage;
    int armor;
    int maxHealth;
    int currentHealth;
    int maxMana;
    int currentMana;
    Attributes attributes;
    Inventory inventory;
    CharacterState state;

public:
    const int STARTING_LEVEL = 1;

    Character(const std::string& id, const std::string& name, const std::string& areaID);

    std::string getName() const;
    std::string getID() const;
    std::string getAreaID() const;
    int getLevel() const;
    int getExp() const;
    int getDamage() const;
    int getArmor() const;
    int getMaxHealth() const;
    int getCurrentHealth() const;
    int getMaxMana() const;
    int getCurrentMana() const;
    Attributes& getAttributes();
    Inventory& getInventory();
    CharacterState getState() const;

    void setID(const std::string& id);
    void setName(const std::string& name);
    void setAreaID(const std::string& areaID);
    void setDamage(int damage);
    void setArmor(int armor);
    void setState(CharacterState state);
    void setLevel(int newLevel);
    void setMaxHealth(int maxHealth);
    void setCurrentHealth(int currentHealth);
    void setMaxMana(int maxMana);
    void setCurrentMana(int currentMana);
    void increaseLevel();
    void increaseExp(int expToAdd);
};

class WarriorCharacter : public Character {
public:
    WarriorCharacter(const std::string& id, const std::string& name, const std::string& areaID)
    : Character(id, name, areaID) {
        attributes.setPrimaryAttribute("strength");
    }

    std::string getClass() {
        return "warrior";
    }
};

class WizardCharacter : public Character {
public:
    WizardCharacter(const std::string& id, const std::string& name, const std::string& areaID)
            : Character(id, name, areaID) {
        attributes.setPrimaryAttribute("intelligence");
    }

    std::string getClass() {
        return "wizard";
    }
};

class RangerCharacter : public Character {
public:
    RangerCharacter(const std::string& id, const std::string& name, const std::string& areaID)
            : Character(id, name, areaID) {
        attributes.setPrimaryAttribute("dexterity");
    }

    std::string getClass() {
        return "ranger";
    }
};

#endif
