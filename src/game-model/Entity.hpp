#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>

class Entity {

public:

    Entity(std::string entityID, std::string name, std::string description) : entityID{entityID}, displayName{displayName}, description{description} {}

    std::string getEntityID() const { return entityID; }
    std::string getDisplayName() const { return displayName; }
    std::string getDescription() const { return description; }

private:

    std::string entityID;
    std::string displayName;
    std::string description;

};

#endif