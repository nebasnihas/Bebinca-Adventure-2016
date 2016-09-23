#include "Area.hpp"

Area::Area() {
	this->id = 0;
	this->areaName = "";
}

Area::Area(int id, std::string areaName, std::vector<int> connectedAreas) {
	this->id = id;
	this->areaName = areaName;
	this->connectedAreas = connectedAreas;
}

std::string Area::getAreaDescription() const {
	// Placeholder function
	return "Temporary description for " + this->areaName + ".";
}