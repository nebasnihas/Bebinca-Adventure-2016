#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>

class Entity {

public:

    Entity(std::string name, std::string description) : name{name}, description{description} {}

    std::string getName() const { return name; }
    std::string getDescription() const { return description; }

private:

    std::string name;
    std::string description;

};

#endif