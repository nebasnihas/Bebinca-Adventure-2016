#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>

class Character 
{

public:

	Character();
	Character(std::string);

	int getID() const { return id; }
	int getAreaID() const { return areaID; }
	std::string getName() const { return name; }

	void setID(int id) { this->id = id; }
	void setAreaID(int areaID) { this->areaID = areaID; }
	void setName(std::string name) { this->name = name; }

private:

	// Id of the character object, may not be needed depending on networking implementation
	int id;
	// Id of the Area object the character is in
	int areaID;
	std::string name;

	// To-do: Add customization features

};

#endif