#include <algorithm>

#include "GameModel.hpp"

bool GameModel::createCharacter(std::string characterID, std::string characterName) {
	Character character(characterID, "area_tutorial", characterName, Character::Type::WARRIOR, 1);
	characters.insert(std::pair<std::string, Character>(characterID, character));

	// No failure case yet...
	return true;
}

bool GameModel::addArea(Area& area) {

	locations.insert(std::pair<std::string, Area>(area.getID(), area));

	// No failure case yet...
	return true;
}

Area* GameModel::getAreaByID(std::string areaID) {
	auto aPtr = locations.find(areaID);

	if (aPtr != locations.end()) {
		return &aPtr->second;
	}

	return nullptr;
}

std::string GameModel::getAreaDescription(std::string areaID) {

	std::string description = "";

	auto area = getAreaByID(areaID);

	if (area != nullptr) {

		description = area->getAreaDescription();
	}

	return description;
}

std::string GameModel::getEntityDescription(std::string areaID, std::string entityDisplayName) {

	auto area = getAreaByID(areaID);

	auto entityList = area->getEntityList();
    auto iter = std::find_if(entityList.begin(), entityList.end(),
                          [&entityDisplayName](const Entity& entity) { return entity.getDisplayName() == entityDisplayName; });
    if (iter != entityList.end()) {
        return iter->getDescription();
    }

    // Error message when entity doesn't exist in area
    return "Entity does not exist.";
}

bool GameModel::moveCharacter(std::string characterID, std::string areaID) {

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

Character* GameModel::getCharacterByID(std::string characterID) {

	auto cPtr = characters.find(characterID);
	
	if (cPtr != characters.end()) {
		return &cPtr->second; 
	}

	return nullptr;
}