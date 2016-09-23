#ifndef AREA_HPP
#define AREA_HPP

#include <string>
#include <vector>

class Area 
{

public:

	Area();
	Area(int, std::string, std::vector<int>);

	int getID() const { return id; }
	std::string getAreaName() const { return areaName; }
	std::vector<int> getConnectedAreas() const { return connectedAreas; }

	std::string getAreaDescription() const;

private:

	// Id of the area object, may not be needed depending on networking implementation
	int id;
	std::string areaName;
	std::vector<int> connectedAreas;

	//TO-DO: Add more detail to an area
};

#endif