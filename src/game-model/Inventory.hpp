#ifndef INVENTORY_H
#define INVENTORY_H

#include <unordered_map>

const int MAX_ITEMS = 30;

class Inventory {
private:
    std::unordered_map<std::string, int> inventory;

public:
    std::string getString() const;
    std::unordered_map<std::string, int> get() const;
    int getSize() const;
    void addItem(const std::string item);
    void removeItem(const std::string item);
    void removeAllItems();
};

#endif