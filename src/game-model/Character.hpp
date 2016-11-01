#ifndef CHARACTER_H
#define CHARACTER_H

#include "Attributes.hpp"
#include "Inventory.hpp"

class Character {
public:
    const int STARTING_LEVEL = 1;

    Character(      const std::string& id,
                    const std::string& name, //Shortdesc is a name
                    std::string& hit,
                    std::string& damage,
                    int level,
                    int exp,
                    int armor,
                    int gold,
                    Inventory inventory,
                    std::string& areaID
                    );

    std::string getName() const;
    std::string getID() const;
    std::string getDamage() const;
    std::string getHit() const;
    int getLevel() const;
    int getExp() const;
    int getArmor() const;
    int getGold() const;
    Inventory& getInventory();
    std::string getAreaID();

    void setID(const std::string& id);
    void setName(const std::string& name);
    void setDamage(const std::string& damage);
    void setArmor(int armor);
    void setGold(int gold);
    void setInventory(const std::string& objectID);
    void setAreaID(const std::string& areaID);

    void setLevel(int newLevel);
    void increaseLevel();
    void increaseExp(int expToAdd);

private:
    std::string id;
    std::string name;
    std::string hit;
    std::string damage;

    int level;
    int exp;
    int armor;
    int gold;

    std::string areaID;
    Inventory inventory;
};


//NPC Subclass
class NPC : public Character{
public:
    NPC(        const std::string& id,
                const std::string& name, //Name is the short desc
                std::string& hit,
                std::string& damage,
                int level,
                int exp,
                int armor,
                const int gold,
                Inventory inventory,
                std::string& areaID,
                const int thac0,
                const std::string& description,
                const std::string& keywords,
                const std::string& longDesc
                );

    std::string getDescription() const;
    std::string getKeywords() const;
    std::string getlongDesc() const;
    int getThac0() const;

private:
    std::string description;
    std::string keywords;
    std::string longDesc;
    int thac0;

};


////Creatable Character Classes
//class WarriorCharacter : public Character {
//public:
//    WarriorCharacter(const std::string& id, const std::string& name, const std::string& areaID)
//    : Character(id, name, areaID) {
//        attributes.setPrimaryAttribute("strength");
//    }
//
//    std::string getClass() {
//        return "warrior";
//    }
//};
//
//class WizardCharacter : public Character {
//public:
//    WizardCharacter(const std::string& id, const std::string& name, const std::string& areaID)
//            : Character(id, name, areaID) {
//        attributes.setPrimaryAttribute("intelligence");
//    }
//
//    std::string getClass() {
//        return "wizard";
//    }
//};
//
//class RangerCharacter : public Character {
//public:
//    RangerCharacter(const std::string& id, const std::string& name, const std::string& areaID)
//            : Character(id, name, areaID) {
//        attributes.setPrimaryAttribute("dexterity");
//    }
//
//    std::string getClass() {
//        return "ranger";
//    }
//
//
//};

#endif
