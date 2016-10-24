#include <iostream>

#include "../../include/game/GameModel.hpp"
#include "../../include/game/Object.hpp"

void addMockCharacters();
void addMockAreas();
void analyzeMovement();

static GameModel gameModel;

int main() 
{
	// Tests
	addMockCharacters();
	addMockAreas();
	analyzeMovement();
	return 0;
}

void addMockCharacters() 
{
	std::cout << "-- Inside Add Mock Characters. --\n\n";

	gameModel.createCharacter("Human", "Josh");

	std::cout << "-- Getting Out of Add Mock Characters. --\n\n";

}

void addMockAreas() 
{
	std::cout << "-- Inside Add Mock Areas. --\n\n";

    /*
     *  TO-DO: Create and initialize Objects here.
     */

    std::cout << "Creating all areas...\n\n";

    /*
     *  TO-DO: Add objects to areas.
     */

	Area a1("Area1", 
			"Home", 
			std::unordered_map<std::string, std::string> {
				{"Area2", "Mall"},
				{"Area3", "Library"}
			}, "This is your home.");

	Area a2("Area2", 
			"Mall", 
			std::unordered_map<std::string, std::string>{
				{"Area1", "Home"}, 
				{"Area3", "Library"}
			}, "This is the mall.");

	Area a3("Area3", 
			"Library", 
			std::unordered_map<std::string, std::string>{
				{"Area2", "Mall"}
			}, "This is the city's library.");

	gameModel.addArea(a1);
	gameModel.addArea(a2);
	gameModel.addArea(a3);

	std::cout << "-- Getting Out Add Mock Areas. --\n\n";
}

void analyzeMovement()
{
	gameModel.setDefaultLocationID("Area0");

	std::cout << "-- Through the rest of the code. --\n\n";

	Character* character = gameModel.getCharacterByID("Human");

	// Used for dev/debug purposes. Use GameModel.moveCharacter for game purposes
	character->setAreaID("Area1");

	std::cout <<"-- PART 1: Moving characters --\n\n";

	bool correctMoveRes = gameModel.moveCharacter("Human", "Area2");
	std::cout << "Moving character to Area2 -- result: " << correctMoveRes 
		<< " character area: " << character->getAreaID() << "\n\n";

	bool redundantMoveRes = gameModel.moveCharacter("Human", "Area2");
	std::cout << "Moving character to Area2 -- result: " << redundantMoveRes
		<< " character area: " << character->getAreaID() << "\n\n";

	bool incorrectMoveRes = gameModel.moveCharacter("Human", "Area0");
	std::cout << "Moving character to Area0 -- result: " << incorrectMoveRes 
		<< " character area: " << character->getAreaID() << "\n\n";

	std::cout <<"-- PART 2: Achieving descriptions --\n\n";

	std::cout << "Character is in Area ID: " << character->getAreaID() << std::endl;

    std::cout << "Getting area description: "
              << gameModel.getAreaDescription(character->getAreaID()) << std::endl;

    std::cout << "Getting Object description of \"rock\": "
              << gameModel.getObjectDescription(character->getAreaID(), "rock") << std::endl;

    std::cout << "\nMoving character to area 1.\n";
    gameModel.moveCharacter("Human", "Area1");

    std::cout << "\nGetting area description: "
              << gameModel.getAreaDescription(character->getAreaID()) << std::endl;

    std::cout << "\nGetting object description of \"rock\": "
              << gameModel.getObjectDescription(character->getAreaID(), "rock") << std::endl;
}
