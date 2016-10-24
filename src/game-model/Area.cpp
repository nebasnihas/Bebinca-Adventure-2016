#include "../../include/game/Area.hpp"

//*---------------
// Constructors
//---------------*

Area::Area(const std::string &id, const std::string &title, const Doors& connectedAreas, const std::string &description)
    : id(id)
    , title(title)
    , connectedAreas(connectedAreas)
    , description(description)
    {
        try
        {
            // TOCHECK: Are the parameters right?
            if (id == "" ||
                title == ""
                )
            {
                throw InvalidAreaException();
            }
        }
        catch(InvalidAreaException& e)
        {
            std::cout << e.what() << std::endl;
        }
    }

Area::Area(const std::string &id, const std::string &title)
    : id(id)
    , title(title)
{
    try
        {
            // TOCHECK: Are the parameters right?
            if (id == "" ||
                title == ""
                )
            {
                throw InvalidAreaException();
            }
        }
        catch(InvalidAreaException& e)
        {
            std::cout << e.what() << std::endl;
        }
}

//*---------------
// Getters
//---------------*


//Accessors
std::string Area::getTitle() const {
    return title;
}

std::string Area::getID() const {
    return id;
}

std::string Area::getDescription() const {
    return description;
}

Area::Doors* Area::getConnectedAreas() const{ 
    return (Doors*) &connectedAreas;
}

std::vector<Object> Area::getObjectList() const {
    return objectList;
}


//TODO : explain this function

/*
 std::string Area::getAreaDescription() const {
 	// Placeholder function
 	std::string description = "";
 	description += "Temporary description for " + this->getTitle() + ".";
 	if (entityList.size() > 0) {
         description += "\nIn this area:\n";
         for (Entity entity : entityList) {
             description += entity.getDisplayName() + "\n";
         }
         description += "\nConnected to the:\n";
         for (auto pair : connectedAreas) {
             description += pair.first + "\n";
         }
     }
     return description;
 }
*/
