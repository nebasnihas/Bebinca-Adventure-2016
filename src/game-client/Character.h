#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include "Inventory.h"

struct Character {
public:
    Inventory inventory;// TODO move to private section

    enum class Type {
        WARRIOR,
        ARCHER,
        MAGE
    };

    Character(string name, Type type);
    string getName() const;
    int getAreaID() const;
    Type getType() const;
    void setAreaID(int areaID);

    //////////////////////////// Level ////////////////////////////

    void increaseLevel();
    int getLevel() const;

    //////////////////////////// Stats ////////////////////////////

    unordered_map<string, int> getStats() const;
    int getStat(string stat);
    void increaseStat(string stat);

    //////////////////////////// Skills ////////////////////////////

    string getSkillsString() const;
    int getSkill(string skill);
    void increaseSkill(string skill);

private:
    string name;
    int areaID;
    Type type;
    int level;
    unordered_map<string, int> stats;
    unordered_map<string, int> skills;
};

#endif
