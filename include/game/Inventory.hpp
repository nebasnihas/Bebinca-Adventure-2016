#ifndef INVENTORY_H
#define INVENTORY_H

#include "Object.hpp"
#include <unordered_map>

const int MAX_ITEMS = 30;
const int MAX_PER_STACKED_ITEM = 20;
const int MAX_WEIGHT = 20;

class Inventory {
private:
    std::vector<Object> inventory;

public:
    Inventory();
    std::string getString() const;
    std::vector<Object> get() const;
    int getSize() const;
    int getWeight() const;
    void addObject(const Object &object);
    void removeObject(const Object &object);
    void removeAllObjects();
};

#endif