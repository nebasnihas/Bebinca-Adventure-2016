#ifndef AREA_HPP
#define AREA_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <exception>

#include "Object.hpp"


class Area 
{

	using Doors =  std::unordered_map<std::string, std::string>;

public:

	//*---------------
	// Constructors
	//---------------*

	Area(const std::string &id, const std::string &title, const Doors& connectedAreas, const std::string &description, const std::vector<std::string>& extendedDescriptions);
	Area(const std::string &id, const std::string &title); //For testing

	//*---------------
	// Getters
	//---------------*

	std::string getID() const;
	std::string getTitle() const;
	std::string getDescription() const;
	Doors* getConnectedAreas() const;
    std::vector<Object> getObjectList() const;
    std::vector<std::string> getExtendedDescriptions() const;

	//std::string getAreaDescription() const;

private:
    // Id of the area object, may not be needed depending on networking implementation

	std::string id;
    std::string title;
	Doors connectedAreas;

    std::string description;
    std::vector<std::string> extendedDescriptions;

	std::vector<Object> objectList; //The Mother Goose YAML file does not contain an object list for every "ROOM".

	//TO-DO: Add more detail to an area
};

/*
 *  VARIOUS AREA EXCEPTIONS
 */

struct InvalidAreaException : public std::exception
{

    virtual const char* what() const throw()
    {
        return "This is an invalid area.";
    }

}; //Invalid character exception

#endif