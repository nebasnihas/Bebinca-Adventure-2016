#include "game/GameModel.hpp"

const std::string ACTION_NPC = "npc";

//Character Defaults
std::string hit = "2d+7";
std::string damage = "1d+9";
int level = 0;
int experience = 0;
int armor = 0;
int gold = 0;
int thac0 = 0;
std::string description = "description";
std::string keywords = "keywords";
std::string longdesc = "longdesc";
Inventory inventory;

bool GameModel::createCharacter(const std::string& characterID, const std::string& characterName) {
    // TO-DO: Placeholder for an initial loading area
	std::string areaID = this->getDefaultLocationID();
	Character character(      characterID,
							  characterName, //Shortdesc is a name
							  hit,
							  damage,
							  level,
							  experience,
							  armor,
							  gold,
							  inventory,
							  areaID
	);
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

void GameModel::createNPC(const std::string& npcID, const std::string& areaID) {
    std::string npcName = "npc name";

    NPC npc(
        npcID,
        npcName,
        hit,
        damage,
        level,
        experience,
        armor,
        gold,
        inventory,
        areaID,
        thac0,
        description,
        keywords,
        longdesc
    );

    npcs.insert(std::pair<std::string, NPC>(npcID, npc));
}

void GameModel::setNPCs(const std::map<std::string, NPC> npcs) {
    this->npcs = npcs;
}

void GameModel::addNPCsToAreas() {
    for (const auto& reset : resets) {
        if (reset.getAction() == ACTION_NPC) {

            for (int i = 0; i < reset.getLimit(); i++) {
                createNPC(reset.getActionID(), reset.getAreaID());
            }
        }
    }
}