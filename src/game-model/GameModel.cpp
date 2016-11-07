#include "game/GameModel.hpp"
#include "combat/CombatManager.hpp"

GameModel::GameModel() {
    // TODO: Move this to something more elegant
    loadDefaultSpells();
}

/*
 *	CHARACTER FUNCTIONS
 */

const std::string ACTION_NPC = "npc";

bool GameModel::createCharacter(const std::string& characterID,
                                const std::string& characterName) {

    // defaults
    std::string hit = NPC::defaultHit;
    std::string damage = NPC::defaultDamage;
    int level = NPC::defaultLevel;
    int armor = NPC::defaultArmor;
    int gold = NPC::defaultGold;
    int experience = NPC::defaultExp;
    Inventory inventory;
    std::string areaID = this->getDefaultLocationID();

	Character character(      characterID,
							  characterName,
							  hit,
							  damage,
							  level,
							  experience,
							  armor,
							  gold,
							  inventory,
							  areaID
	);
	auto outputBuffer = std::make_shared<std::deque<std::string>>();
	character.setOutputBuffer(outputBuffer);

	characters.insert(std::pair<std::string, Character>(characterID, character));

	// Possible failure cases
	// - Invalid character; taken care of by the Character class

	return true;
}

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

bool GameModel::addArea(const Area area) {

	locations.insert(std::pair<std::string, Area>(area.getID(), std::move(area)));

	// No failure case yet...
	return true;
}

Area* GameModel::getAreaByID(const std::string& areaID) const {

    if (locations.count(areaID) == 1) {

        return (Area*)&(locations.at(areaID));
    }

	return nullptr;
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

std::string GameModel::getEntityDescription(const std::string& areaID, const std::string& entityDisplayName) const {

	auto area = getAreaByID(areaID);

	auto entityList = area->getObjectList();
    auto iter = std::find_if(entityList.begin(), entityList.end(),
                          [&entityDisplayName](const Object& entity) { return entity.getName() == entityDisplayName; });
    if (iter != entityList.end()) {
        return iter->getDescription();
    }

    // Error message when entity doesn't exist in area
    return "Entity does not exist.";
}

bool GameModel::moveCharacter(const std::string& characterID, const std::string& areaTag) {

    auto character = getCharacterByID(characterID);
    if (character == nullptr) {
        return false;
    }

    if (!characterCanMove(*character)) {
        return false;
    }

    auto area = getAreaByID(character->getAreaID());
    if (area == nullptr) {
        return false;
    }

    auto connectedAreas = area->getConnectedAreas();
    // Check if the current area is connected to the target destination
    auto connectedArea = connectedAreas->find(areaTag);

    if (connectedArea == connectedAreas->end()) {
        return false;
    }

    character->setAreaID(connectedArea->second);
    return true;

}

bool GameModel::characterCanMove(const Character& character) {
    return character.getState() == CharacterState::IDLE;
}

bool GameModel::characterIsInCombat(const std::string& characterID) {
    auto character = getCharacterByID(characterID);
    return character->getState() == CharacterState::BATTLE;
}

bool GameModel::characterIsDead(const std::string& characterID) {
    auto character = getCharacterByID(characterID);
    return character->getState() == CharacterState::DEAD;
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

Character* GameModel::getCharacterByID(const std::string& characterID) const {
	if (characters.count(characterID) == 1) {
        auto character = (Character*)&(characters.at(characterID));
        return getBodySwappedCharacter(character);
    }

	return nullptr;
}

Character* GameModel::getBodySwappedCharacter(Character* character) const {

    auto& statusEffects = character->getStatusEffects();
    auto statusEffect = std::find_if(statusEffects.begin(), statusEffects.end(),
                                     [] (const auto& se) { return se->getType() == StatusType::BODYSWAP; });
    if (statusEffect == statusEffects.end()) {
        return character;
    } else {
        // TODO: print bodyswapped message
        return getCharacterByID(std::static_pointer_cast<BodySwapStatus>(*statusEffect)->getSwappedID());
    }

}

void GameModel::addNPCsToAreas() {
    for (const auto& reset : resets) {

        if (reset.getAction() == ACTION_NPC) {
            try {

                auto& npc = npcTemplates.at(reset.getActionID());

                for (int i = 0; i < reset.getLimit(); i++) {
                    std::string newNpcID = npc.getID();
                    newNpcID = newNpcID.append(std::to_string(npc.getCounter()));

                    auto newNPC = NPC(npc);
                    npc.setID(newNpcID);
                    npc.setAreaID(reset.getAreaID());
                    npcs.insert(std::pair<std::string, NPC>(newNPC.getID(), newNPC));
                    npc.increaseCounter();
                }

            } catch (std::out_of_range ex) {

            }
        }
    }
}

void GameModel::setNPCs(const std::unordered_map<std::string, NPC> npcs) {
    this->npcs = npcs;
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

std::vector<std::string> GameModel::getPossibleTargets(const std::string& characterID) {
    auto combatInstance = combatManager.getCombatInstanceByCharacterID(characterID);
    return combatInstance->getPossibleTargets(characterID);
}

void GameModel::update() {

    // Manage dead characters
    //manageDeadCharacters();

    if (gameTicks % GameModel::GAME_TICKS_PER_COMBAT_TICK == 0) {
        // Update all the combat instances
        combatManager.update();
    }

    // Consider not calling this every tick
    updateStatusEffects();

    gameTicks++;
}

void GameModel::manageDeadCharacters() {
    for (auto& pair : characters) {
        auto character = pair.second;
        if (character.getState() == CharacterState::DEAD) {
            // TODO: Add NPC Clause
            character.setAreaID(getDefaultLocationID());
            character.setState(CharacterState::IDLE);
        }
    }
}

// TODO: Move this dependency to different functions
void GameModel::loadActions(const std::unordered_map<std::string, std::shared_ptr<CombatAction>>& actionLookup) {
    combatManager.setActionLookup(actionLookup);
}

void GameModel::addSpell(Spell spell) {
    auto spellName = spell.getName();
    spells.insert({ spellName, std::move(spell) });
    Spell* spellRef = &(spells.at(spellName));
    combatManager.addSpellAction(*spellRef);
}

void GameModel::updateStatusEffects() {

    time_t currentTime;
    time(&currentTime);

    for (auto& pair : characters) {
        auto& character = pair.second;
        auto& statusEffects = character.getStatusEffects();

        // Remove if we have passed the end time
        auto eraseIter = std::remove_if(statusEffects.begin(), statusEffects.end(),
                                        [&currentTime] (const auto& se) {
                                            return se->getEndTime() < currentTime;
                                        }
        );
        statusEffects.erase(eraseIter, statusEffects.end());
    }
}

void GameModel::pushToOutputBuffer(const std::string& characterID, std::string message) {
	getCharacterByID(characterID)->pushToBuffer(message);
}

void GameModel::listValidSpells(const std::string& characterID) {
	auto character = getCharacterByID(characterID);
	std::string message = "List of Spells:\n";
	for (auto& spellPairs: spells) {
		message += spellPairs.first + ", ";
	}
	character->pushToBuffer(message);
}

void GameModel::loadDefaultSpells() {
    Spell bodySwap("bodyswap", 0, SpellType::BODY_SWAP, "");
    addSpell(bodySwap);
}