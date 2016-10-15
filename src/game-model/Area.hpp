#ifndef AREA_HPP
#define AREA_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include "Entity.hpp"

class Area 
{

	typedef std::unordered_map<std::string, std::string> Doors;

public:

	Area(const std::string &id, const std::string &title, Doors connectedAreas, const std::string &description);

	//for testing
	Area(const std::string &id, const std::string &title);

	std::string getID() const;
	std::string getTitle() const;
	std::string getDescription() const;
	Doors* getConnectedAreas() const;
    



    std::vector<Entity> getEntityList() const { return entityList; }

	// std::string getAreaDescription() const;




    //TO-DO: Add loading mechanism from file
    static Area* loadAreaFromFile(std::string);

private:
    // Id of the area object, may not be needed depending on networking implementation


	std::string id;
    std::string title;
	Doors connectedAreas;

    std::string description;
	std::vector<Entity> entityList;

	//TO-DO: Add more detail to an area
};

#endif