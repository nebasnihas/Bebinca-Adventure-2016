#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include "Inventory.hpp"

class Character {
public:
    Inventory inventory;// TODO move to private section

    enum class Type {
        WARRIOR,
        ARCHER,
        MAGE
    };

    Character(int id, int areaID, std::string name, Type type, int level);
    std::string getName() const;
    int getID() const;
    int getAreaID() const;
    Type getType() const;
    void setID(int id);
    void setAreaID(int areaID);

    //////////////////////////// Level ////////////////////////////

    void increaseLevel();
    int getLevel() const;

    //////////////////////////// Stats ////////////////////////////

    std::unordered_map<std::string, int> getStats() const;
    int getStat(std::string stat);
    void increaseStat(std::string stat);

    //////////////////////////// Skills ////////////////////////////

    std::string getSkillsString() const;
    int getSkill(std::string skill);
    void increaseSkill(std::string skill);

private:
    std::string name;
    int id;
    int areaID;
    Type type;
    int level;
    std::unordered_map<std::string, int> stats;
    std::unordered_map<std::string, int> skills;
};

#endif
