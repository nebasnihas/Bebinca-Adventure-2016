#include <iostream>

#include "GameModel.hpp"
#include "Entity.hpp"

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

	bool redundantMoveRes = gameModel.moveCharacter(1, 2);
	std::cout << "Moving character to area 2 --result: " << redundantMoveRes
		<< " character area: " << character->getAreaID() << std::endl;

	bool incorrectMoveRes = gameModel.moveCharacter(1, 0);
	std::cout << "Moving character to area 0 --result: " << incorrectMoveRes 
		<< " character area: " << character->getAreaID() << std::endl;

    std::cout << "\nGetting area description: "
              << gameModel.getAreaDescription(character->getAreaID()) << std::endl;

    std::cout << "\nGetting entity description of \"rock\": "
              << gameModel.getEntityDescription(character->getAreaID(), "rock") << std::endl;

    std::cout << "\nMoving character to area 1.\n";
    gameModel.moveCharacter(1, 1);

    std::cout << "\nGetting area description: "
              << gameModel.getAreaDescription(character->getAreaID()) << std::endl;

    std::cout << "\nGetting entity description of \"rock\": "
              << gameModel.getEntityDescription(character->getAreaID(), "rock") << std::endl;
}

void addMockCharacters() 
{
	Character c1(1, 1, "Test Character", Character::Type::WARRIOR, 1);
	gameModel.createCharacter(c1);

}

void addMockAreas() 
{
    Entity tree = Entity("tree", "This is a tree.");
    Entity rock = Entity("rock", "This is a rock.");
    Entity test = Entity("test", "This is a test.");

	Area a1(1, "Area1", std::vector<int>{2, 3}, std::vector<Entity>{});
	gameModel.addArea(a1);

	Area a2(2, "Area2", std::vector<int>{1, 3}, std::vector<Entity>{tree, rock});
	gameModel.addArea(a2);

	Area a3(3, "Area3", std::vector<int>{2}, std::vector<Entity>{test});
	gameModel.addArea(a3);
}