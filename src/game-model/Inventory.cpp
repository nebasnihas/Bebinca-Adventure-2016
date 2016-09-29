#include <iostream>
#include "Inventory.hpp"

std::string Inventory::getString() const {
    std::string inventoryString = "";

    if (getSize() > 0) {
        for (const auto& item : inventory) {
            inventoryString += item.first + " x" + std::to_string(item.second) + "\n";
        }
    } else {
        inventoryString = "Your inventory is empty.";
    }

    return inventoryString;
}

std::unordered_map<std::string, int> Inventory::get() const {
    return inventory;
}

int Inventory::getSize() const {
    int size = 0;

    for (const auto& item : inventory) {
        int itemCount = item.second;
        size += itemCount;
    }

    return size;
}

void Inventory::addItem(const std::string item) {
    if (getSize() < MAX_ITEMS) {
        inventory[item]++;
    }
}

void Inventory::removeItem(const std::string item) {
    int itemCount = inventory[item];

    if (itemCount > 1) {
        inventory[item]--;
    } else {
        inventory.erase(item);
    }
}

void Inventory::removeAllItems() {
    inventory.clear();
}
