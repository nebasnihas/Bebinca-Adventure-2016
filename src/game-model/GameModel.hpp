#ifndef GAME_MODEL_HPP
#define GAME_MODEL_HPP

#include <map>

#include "Area.hpp"
#include "Character.hpp"

class GameModel {

public:

	bool createCharacter(Character&);
	bool moveCharacter(int, int);
	Character* getCharacterByID(int);

	bool addArea(Area&);
	Area* getAreaByID(int);

	std::string getAreaDescription(int);
	std::string getEntityDescription(int, std::string);

private:

	int getNextOpenCharacterID() const;

	std::map<int, Character> characters;
	std::map<int, Area> locations;

};

#endif