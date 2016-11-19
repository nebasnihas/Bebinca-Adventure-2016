#include <iostream>
#include "../../include/game/Inventory.hpp"

Inventory::Inventory() {
    inventory = std::vector<Object>();
}

std::string Inventory::getString() const {
    std::string inventoryString = "";

    if (!inventory.empty()) {
        for (const auto& object : inventory) {
            inventoryString += object.getName() + "\n";
        }
    } else {
        inventoryString = "Your inventory is empty.";
    }

    return inventoryString;
}

std::vector<Object> Inventory::get() const {
    return inventory;
}

int Inventory::getSize() const {
    return inventory.size();
}

int Inventory::getWeight() const {
    int weight = 0;

    for (const auto& object : inventory) {
        weight += object.getWeight();
    }

    return weight;
}

void Inventory::addObject(const Object &object) {
    if ((getWeight() + object.getWeight()) < MAX_WEIGHT) {
        inventory.push_back(object);
    }
}

void Inventory::removeObject(const Object &object) {

}

void Inventory::removeAllObjects() {
    inventory.clear();
}
