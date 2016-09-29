#include <iostream>
#include "Inventory.h"

string Inventory::getString() const {
    string inventoryString = "";

    if (getSize() > 0) {
        for (const auto& item : inventory) {
            inventoryString += item.first + " x" + to_string(item.second) + "\n";
        }
    } else {
        inventoryString = "Your inventory is empty.";
    }

    return inventoryString;
}

unordered_map<string, int> Inventory::get() const {
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

void Inventory::addItem(const string item) {
    if (getSize() < MAX_ITEMS) {
        inventory[item]++;
    }
}

void Inventory::removeItem(const string item) {
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
