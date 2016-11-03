#include <game/GameModel.hpp>

/*
 *	CHARACTER FUNCTIONS
 */

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

    // Placeholder for an initial loading area
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

	// Possible failure cases
	// - Invalid character; taken care of by the Character class

	return true;
}

bool GameModel::moveCharacter(const std::string& characterID, const std::string& areaTag) {

	try
    {
        // PART A - Find and validate the character inside the character map.
		auto cPtr = characters.find(characterID);
		if ( ! ( cPtr != characters.end() ) ) {
			throw InvalidMoveException();
			return false;
		}

		// PART B - Find and validate the character's current location
		// inside the location map.
		Character* currentCharacter = &cPtr->second;
		auto aPtr = locations.find ( currentCharacter -> getAreaID() );
		if ( ! ( aPtr != locations.end() ) ) {
			throw InvalidMoveException();
			return false;
		}

		// PART C - Check and validate that the character's target destination
		// is connected to his/her current location.
		Area currentArea = aPtr->second;
		auto connectedAreas = currentArea.getConnectedAreas();
		auto targetArea = connectedAreas->find ( areaTag ) ;
	    if ( ! ( targetArea != connectedAreas->end() ) ) {
	    	throw InvalidMoveException();
	    	return false;
	    }

	    // PART D - Move the character by changing its area ID.
		currentCharacter->setAreaID(targetArea->first);
		return true;
    }
    catch(InvalidMoveException& e)
    {
        std::cout << e.what() << std::endl;
    }
}

Character* GameModel::getCharacterByID(const std::string& characterID) const {

	if (characters.count(characterID) != 1) {
		return nullptr;
	}

	return (Character*)&(characters.at(characterID));
}

/*
 *	OBJECT FUNCTIONS
 */

std::string GameModel::getObjectDescription(const std::string& areaID, const std::string& objectName) const {

	auto area = getAreaByID(areaID);
	auto objectList = area->getObjectList();
    auto objectIter = std::find_if(objectList.begin(), objectList.end(),
                          [&objectName](const Object& object) { return object.getName() == objectName; });

    // Error message when entity doesn't exist in area
    if ( ! (objectIter != objectList.end() ) ) {
    	return "Object does not exist.";
    }

    return objectIter->getDescription();
}

/*
 *	AREA FUNCTIONS
 */

bool GameModel::addArea(const Area& area) {

	locations.insert(std::pair<std::string, Area>(area.getID(), std::move(area)));

	// No failure case yet...
	return true;
}

Area* GameModel::getAreaByID(const std::string& areaID) const {

    if ( locations.count(areaID) != 1 ) {
		return nullptr;
	}

	return (Area*)&(locations.at(areaID));
}

std::string GameModel::getAreaDescription(const std::string& areaID) const {

	auto area = getAreaByID(areaID);

	if (area == nullptr) {
		return "Area does not exist.\n";
	}

	return area->getDescription();

}

std::unordered_map<std::string, std::string>* GameModel::getConnectedAreas(const std::string& areaID) const {
    auto area = getAreaByID(areaID);
    return area->getConnectedAreas();
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

/*
 *	LOCATION FUNCTIONS
 */

std::string GameModel::getDefaultLocationID() const {
	return this->defaultLocation;
}

void GameModel::setDefaultLocationID(const std::string& locationID) {
	this->defaultLocation = locationID;
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
//    for (const auto& reset : resets) {
//        if (reset.getAction() == ACTION_NPC) {
//
//            for (int i = 0; i < reset.getLimit(); i++) {
//                createNPC(reset.getActionID(), reset.getAreaID());
//            }
//        }
//    }
}