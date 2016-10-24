#include "Area.hpp"


Area::Area(const std::string &id, const std::string &title, Doors connectedAreas, const std::string &description)
    : id(id)
    , title(title)
    , connectedAreas(connectedAreas)
    , description(description)
{ }

Area::Area(const std::string &id, const std::string &title)
    : id(id)
    , title(title)
{ }


//Accessors
std::string Area::getTitle() const { return title; }

std::string Area::getID() const { return id; }

std::string Area::getDescription() const { return description; }

Area::Doors* Area::getConnectedAreas() const
{ 
    return (Doors*) &connectedAreas;
}


//TODO : explain this function

// std::string Area::getAreaDescription() const {
// 	// Placeholder function
// 	std::string description = "";
// 	description += "Temporary description for " + this->getTitle() + ".";

// 	if (entityList.size() > 0) {

//         description += "\nIn this area:\n";

//         for (Entity entity : entityList) {
//             description += entity.getDisplayName() + "\n";
//         }

//         description += "\nConnected to the:\n";

//         for (auto pair : connectedAreas) {
//             description += pair.first + "\n";
//         }
//     }

//     return description;
// }
// }

// Area* Area::loadAreaFromFile(std::string filePath) {
//     // TO-DO: Implement function once file format has been decided
//     //        Prefer loading from file contents instead of filepath if possible
//     //        Change in function signature will be needed

// }
