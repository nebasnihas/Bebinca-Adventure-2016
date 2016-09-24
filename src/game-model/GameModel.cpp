#include <algorithm>

#include "GameModel.hpp"

bool GameModel::createCharacter(Character& character) {
	int openID = getNextOpenCharacterID();
	character.setID(openID);
	characters.insert(std::pair<int, Character>(openID, character));

	// No failure case yet...
	return true;
}

int GameModel::getNextOpenCharacterID() const {

	// Placeholder functionality for the current build.
	// Will need to be changed when players are stored
	int maxID = 1;
	for (auto pair : characters) {
		if (pair.first > maxID) {
			maxID = pair.first;
		}
	}
	return maxID++;
}

bool GameModel::addArea(Area& area) {

	locations.insert(std::pair<int, Area>(area.getID(), area));

	// No failure case yet...
	return true;
}

Area* GameModel::getAreaByID(int areaID) {
	auto aPtr = locations.find(areaID);

	if (aPtr != locations.end()) {
		return &aPtr->second;
	}

	return nullptr;
}

std::string GameModel::getAreaDescription(int areaID) {

	std::string description = "";

	auto area = getAreaByID(areaID);

	if (area != nullptr) {

		description = area->getAreaDescription();
	}

	return description;
}

std::string GameModel::getEntityDescription(int areaID, std::string entityName) {

	auto area = getAreaByID(areaID);

	auto entityList = area->getEntityList();
    auto iter = std::find_if(entityList.begin(), entityList.end(),
                          [&entityName](const Entity& entity) { return entity.getName() == entityName; });
    if (iter != entityList.end()) {
        return iter->getDescription();
    }

    // Error message when entity doesn't exist in area
    return "Entity does not exist.";
}

bool GameModel::moveCharacter(int characterID, int areaID) {

	auto cPtr = characters.find(characterID);
	
	if (cPtr != characters.end()) {
		
		Character* character = &cPtr->second;
		auto aPtr = locations.find(character->getAreaID());
		
		if (aPtr != locations.end()) {

			Area area = aPtr->second;

			auto connectedAreas = area.getConnectedAreas();
			
			// Check if the current area is connected to the target destination
			if (std::find(connectedAreas.begin(), connectedAreas.end(), areaID)
				!= connectedAreas.end()) {

				character->setAreaID(areaID);
				return true;
			}	
		}

	}

	return false;
}

Character* GameModel::getCharacterByID(int characterID) {

	auto cPtr = characters.find(characterID);
	
	if (cPtr != characters.end()) {
		return &cPtr->second; 
	}

	return nullptr;
}