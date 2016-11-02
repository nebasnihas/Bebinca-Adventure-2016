#include "game/GameModel.hpp"
#include "combat/CombatManager.hpp"

bool GameModel::createCharacter(const std::string& characterID, const std::string& characterName) {
    // TO-DO: Placeholder for an initial loading area
	std::string areaID = this->getDefaultLocationID();
	Character character(characterID, characterName, areaID);
	characters.insert(std::pair<std::string, Character>(characterID, character));

	// No failure case yet...
	return true;
}

bool GameModel::addArea(const Area area) {

	locations.insert(std::pair<std::string, Area>(area.getID(), std::move(area)));

	// No failure case yet...
	return true;
}

std::string GameModel::getDefaultLocationID() const {
	return this->defaultLocation;
}

void GameModel::setDefaultLocationID(const std::string& locationID) {
	this->defaultLocation = locationID;
}

Area* GameModel::getAreaByID(const std::string& areaID) const {

    if (locations.count(areaID) == 1) {

        return (Area*)&(locations.at(areaID));
    }

	return nullptr;
}

std::string GameModel::getAreaDescription(const std::string& areaID) const {

	std::string description = "";

	auto area = getAreaByID(areaID);

	if (area != nullptr) {

		description = area->getDescription();
	}

	return description;
}

std::string GameModel::getEntityDescription(const std::string& areaID, const std::string& entityDisplayName) const {

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

bool GameModel::moveCharacter(const std::string& characterID, const std::string& areaTag) {

	auto cPtr = characters.find(characterID);
	
	if (cPtr != characters.end()) {
		
		Character* character = &cPtr->second;
		auto aPtr = locations.find(character->getAreaID());
		
		if (aPtr != locations.end()) {

			Area area = aPtr->second;

			auto connectedAreas = area.getConnectedAreas();
			
			// Check if the current area is connected to the target destination
			auto connectedArea = connectedAreas->find(areaTag);

            if (connectedArea != connectedAreas->end()) {

				character->setAreaID(connectedArea->second);
				return true;
			}	
		}

	}

	return false;
}

std::vector<std::string> GameModel::getCharacterIDsInArea(const std::string& areaID) const {
	Area* area = getAreaByID(areaID);

	std::vector<std::string> charactersInArea;
	//TODO consider keep track of character inside area class
	for (const auto& pair : characters) {

		auto character = pair.second;
		if (character.getAreaID() == areaID) {
			charactersInArea.push_back(character.getID());
		}
	}

	return charactersInArea;
}

std::unordered_map<std::string, std::string>* GameModel::getConnectedAreas(const std::string& areaID) const {
    auto area = getAreaByID(areaID);
    return area->getConnectedAreas();
}

Character* GameModel::getCharacterByID(const std::string& characterID) const {

	if (characters.count(characterID) == 1) {
        return (Character*)&(characters.at(characterID));
    }

	return nullptr;
}

bool GameModel::engageCharacterInCombat(const std::string& characterID, const std::string& target) {
    auto c1 = getCharacterByID(characterID);
    auto c2 = getCharacterByID(target);

    if (c1 == nullptr || c2 == nullptr) {
        return false;
    }

    auto battleInstance = combatManager.getNewCombatInstance();
    battleInstance.addCharacterToNewTeam(*c1);
    battleInstance.addCharacterToNewTeam(*c2);
    combatManager.loadCombatInstance(battleInstance);

    return true;
}

bool GameModel::setCombatAction(const std::string& characterID, const std::string& actionName) {
    auto characterInstance = combatManager.getCharacterInstanceByCharacterID(characterID);
    if (characterInstance == nullptr) {
        return false;
    }
    characterInstance->setCombatActionID(actionName);
    return true;
}

bool GameModel::setCombatTarget(const std::string& characterID, const std::string& targetID) {
    auto combatInstance = combatManager.getCombatInstanceByCharacterID(characterID);

    if (combatInstance == nullptr) {
        return false;
    }

    auto characterInstance = combatInstance->getCharacterInstance(characterID);
    auto targetInstance = combatInstance->getCharacterInstance(targetID);

    if (targetInstance == nullptr) {
        return false;
    }

    characterInstance->setTarget(*targetInstance);
    return true;

}