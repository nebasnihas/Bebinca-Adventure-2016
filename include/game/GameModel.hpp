#ifndef GAME_MODEL_HPP
#define GAME_MODEL_HPP

#include <map>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <exception>

#include <game/Area.hpp>
#include <game/Character.hpp>
#include "../../src/game-model/Resets.hpp"
#include "Object.hpp"
#include <game/Inventory.hpp>

class GameModel {

public:

	/*
	 *	CHARACTER FUNCTIONS
	 */

	bool createCharacter(      const std::string& characterID,
                               const std::string& characterName //Shortdesc is a name
                        );

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



    void createNPC(const std::string& npcID,
                   const std::string& name, //Name is the short desc
                   std::string& hit,
                   std::string& damage,
                   int level,
                   int exp,
                   int armor,
                   int gold,
                   Inventory inventory,
                   std::string& areaID,
                   int thac0,
                   const std::string& description,
                   const std::string& keywords,
                   const std::string& longDesc);

    void setNPCs(const std::map<std::string, NPC> npcs);
    void addNPCsToAreas();

private:

	std::map<std::string, Character> characters;
	std::map<std::string, Area> locations;
    std::map<std::string, NPC> npcs;
    std::vector<Resets> resets;

    std::unordered_map<std::string, NPC> npcTemplates;

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