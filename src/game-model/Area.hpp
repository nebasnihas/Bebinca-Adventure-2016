#ifndef AREA_HPP
#define AREA_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include "Entity.hpp"
#include "GameDataImporter.hpp"

class Area 
{

public:

	Area(std::string id, std::string areaName, std::unordered_map<std::string, std::string> connectedAreas, std::vector<std::string> description);

	std::string getID() const { return id; }
	std::string getAreaName() const { return areaName; }
    std::unordered_map<std::string, std::string>* getConnectedAreas() const { return (std::unordered_map<std::string, std::string>*)&connectedAreas; }
    std::vector<Entity> getEntityList() const { return entityList; }

	std::string getAreaDescription() const;

    //TO-DO: Add loading mechanism from file
    static Area* loadAreaFromFile(std::string);

private:
    // Id of the area object, may not be needed depending on networking implementation
	std::string id;
    std::string areaName;
    std::unordered_map<std::string, std::string> connectedAreas;

	std::vector<Entity> entityList;

	//TO-DO: Add more detail to an area
};

#endif