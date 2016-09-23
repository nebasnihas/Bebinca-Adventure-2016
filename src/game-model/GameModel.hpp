#ifndef GAME_MODEL_HPP
#define GAME_MODEL_HPP

#include <map>

#include "Area.hpp"
#include "Character.hpp"

class GameModel {

public:

	bool addCharacter(Character&);
	bool moveCharacter(int, int);

	bool addArea(Area&);

	Character* getCharacterByID(int);

private:

	int getNextOpenCharacterID() const;

	std::map<int, Character> characters;
	std::map<int, Area> locations;

};

#endif