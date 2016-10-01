#ifndef AREA_HPP
#define AREA_HPP

#include <string>
#include <vector>

#include "Entity.hpp"

class Area 
{

public:

	Area();
	Area(int, std::string, std::vector<int>, std::vector<Entity>);

	int getID() const { return id; }
	std::string getAreaName() const { return areaName; }
	std::vector<int> getConnectedAreas() const { return connectedAreas; }
    std::vector<Entity> getEntityList() const { return entityList; }

	std::string getAreaDescription() const;

    //TO-DO: Add loading mechanism from file
    static Area* loadAreaFromFile(std::string);

private:
    // Id of the area object, may not be needed depending on networking implementation
	int id;
    std::string areaName;
    std::vector<int> connectedAreas;

	std::vector<Entity> entityList;

	//TO-DO: Add more detail to an area
};

#endif