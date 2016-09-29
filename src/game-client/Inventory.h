#ifndef INVENTORY_H
#define INVENTORY_H

#include <unordered_map>

using namespace std;

const int MAX_ITEMS = 30;

class Inventory {
private:
    unordered_map<string, int> inventory;

public:
    string getString() const;
    unordered_map<string, int> get() const;
    int getSize() const;
    void addItem(const string item);
    void removeItem(const string item);
    void removeAllItems();
};

#endif