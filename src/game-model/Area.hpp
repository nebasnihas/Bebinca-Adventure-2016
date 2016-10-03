#ifndef AREA_HPP
#define AREA_HPP

#include <string>
#include <vector>

#include "Entity.hpp"

class Area 
{

public:

	Area(std::string id, std::string areaName, std::vector<std::string> connectedAreas, std::vector<Entity> entityList);

	std::string getID() const { return id; }
	std::string getAreaName() const { return areaName; }
	std::vector<std::string> getConnectedAreas() const { return connectedAreas; }
    std::vector<Entity> getEntityList() const { return entityList; }

	std::string getAreaDescription() const;

    //TO-DO: Add loading mechanism from file
    static Area* loadAreaFromFile(std::string file);

private:
    // Id of the area object, may not be needed depending on networking implementation
	std::string id;
    std::string areaName;
    std::vector<std::string> connectedAreas;

	std::vector<Entity> entityList;

	//TO-DO: Add more detail to an area
};

#endif