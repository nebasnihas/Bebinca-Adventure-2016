#ifndef GAME_MODEL_HPP
#define GAME_MODEL_HPP

#include <map>

#include "Area.hpp"
#include "Character.hpp"

class GameModel {

public:

	bool createCharacter(std::string characterID, std::string characterName);
	bool moveCharacter(std::string characterID, std::string areaID);
	Character* getCharacterByID(std::string characterID);

    bool addArea(Area& area);

	Area* getAreaByID(std::string areaID);
    std::string getAreaDescription(std::string areaID);

	std::string getEntityDescription(std::string areaID, std::string entityID);

private:

	std::map<std::string, Character> characters;
	std::map<std::string, Area> locations;

};

#endif