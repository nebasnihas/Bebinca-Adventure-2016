#ifndef GAME_MODEL_HPP
#define GAME_MODEL_HPP

#include <map>
#include <algorithm>
#include <utility>
#include <exception>

#include "Area.hpp"
#include "Character.hpp"
#include "Object.hpp"

class GameModel {

public:

	/*
	 *	CHARACTER FUNCTIONS
	 */

	bool createCharacter(const std::string& characterID, const std::string& characterName);
	bool moveCharacter(const std::string& characterID, const std::string& areaTag);
	Character* getCharacterByID(const std::string& characterID) const;

	/*
	 *	OBJECT FUNCTIONS
	 */

	std::string getObjectDescription(const std::string& areaID, const std::string& objectName) const;

	/*
	 *	AREA FUNCTIONS
	 */

  	bool addArea(const Area& area);
  	Area* getAreaByID(const std::string& areaID) const;
  	std::string getAreaDescription(const std::string& areaID) const;
  	std::unordered_map<std::string, std::string>* getConnectedAreas(const std::string& areaID) const;
	std::vector<std::string> getCharacterIDsInArea(const std::string& areaID) const;

	/*
	 *	LOCATION FUNCTIONS
	 *	TO-DO: Make the location an actual area.
	 */

	std::string getDefaultLocationID() const;
	void setDefaultLocationID(const std::string& locationID);
	


private:

	std::map<std::string, Character> characters;
	std::map<std::string, Area> locations;
	std::string defaultLocation;

}; //GameModel class


/*
 *  VARIOUS GAME MODEL EXCEPTIONS
 */

struct InvalidMoveException : public std::exception
{

    virtual const char* what() const throw()
    {
        return "This move is invalid.";
    }

}; //Invalid character exception

#endif