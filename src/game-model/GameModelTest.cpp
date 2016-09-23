#include <iostream>

#include "GameModel.hpp"

void addMockCharacters();
void addMockAreas();

static GameModel gameModel;

int main() 
{
	// Tests
	addMockCharacters();
	addMockAreas();

	Character* character = gameModel.getCharacterByID(1);

	// Used for dev/debug purposes. Use GameModel.moveCharacter for game purposes
	character->setAreaID(1);

	bool correctMoveRes = gameModel.moveCharacter(1, 2);
	std::cout << "Moving character to area 2 --result: " << correctMoveRes 
		<< " character area: " << character->getAreaID() << std::endl;

	bool redunantMoveRes = gameModel.moveCharacter(1, 2);
	std::cout << "Moving character to area 2 --result: " << redunantMoveRes 
		<< " character area: " << character->getAreaID() << std::endl;

	bool incorrectMoveRes = gameModel.moveCharacter(1, 0);
	std::cout << "Moving character to area 0 --result: " << incorrectMoveRes 
		<< " character area: " << character->getAreaID() << std::endl;
}

void addMockCharacters() 
{
	Character c1("Test Character");
	gameModel.addCharacter(c1);

}

void addMockAreas() 
{
	Area a1(1, "Area1", std::vector<int>{2, 3});
	gameModel.addArea(a1);

	Area a2(2, "Area2", std::vector<int>{1, 3});
	gameModel.addArea(a2);

	Area a3(3, "Area3", std::vector<int>{2});
	gameModel.addArea(a3);
}