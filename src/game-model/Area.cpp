#include "Area.hpp"

Area::Area(std::string id, std::string areaName, std::vector<std::string> connectedAreas, std::vector<Entity> entityList) {
	this->id = id;
	this->areaName = areaName;
	this->connectedAreas = connectedAreas;
	this->entityList = entityList;
}

std::string Area::getAreaDescription() const {
	// Placeholder function
	std::string description = "";
	description += "Temporary description for " + this->getAreaName() + ".";

	if (entityList.size() > 0) {

        description += "\nIn this area:\n";

        for (Entity entity : entityList) {
            description += entity.getDisplayName() + "\n";
        }
    }

    return description;
}

Area* Area::loadAreaFromFile(std::string filePath) {
    // TO-DO: Implement function once file format has been decided
    //        Prefer loading from file contents instead of filepath if possible
    //        Change in function signature will be needed

}
