#include "../../include/game/Area.hpp"

//*---------------
// Constructors
//---------------*

Area::Area( const std::string &id, 
            const std::string &title, 
            const Doors& connectedAreas, 
            const std::string &description,
			const std::unordered_map<std::string, std::string>& extendedDescriptions)
    : id(id)
    , title(title)
    , connectedAreas(connectedAreas)
    , description(description)
    , extendedDescriptions(extendedDescriptions)
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

std::unordered_map<std::string, std::string> Area::getExtendedDescriptions() const {
    return extendedDescriptions;
}

//Setters
void Area::addObjects(std::string object) {
    this->objectNames.push_back(object);
}

const std::vector<std::string>& Area::getObjectNames() const{
	return objectNames;
}