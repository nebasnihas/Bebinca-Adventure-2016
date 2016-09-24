#include "Area.hpp"

Area::Area() {
	this->id = 0;
	this->areaName = "";
}

Area::Area(int id, std::string areaName, std::vector<int> connectedAreas, std::vector<Entity> entityList) {
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
            description += entity.getName() + "\n";
        }
    }

    return description;
}

Area* Area::loadAreaFromFile(std::string filePath) {
    // TO-DO: Implement function once file format has been decided
    //        Prefer loading from file contents instead of filepath if possible
    //        Change in function signature will be needed

}
