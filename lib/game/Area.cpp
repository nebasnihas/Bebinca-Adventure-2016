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
