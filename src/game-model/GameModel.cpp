#include <boost/format.hpp>
#include "game/GameModel.hpp"
#include "combat/CombatManager.hpp"
#include "GameStrings.hpp"

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
	auto outputBuffer = std::make_shared<std::deque<PlayerMessage>>();
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

std::vector<std::string> GameModel::getNPCIDsInArea(const std::string &areaID) const {
	std::vector<std::string>NPCsInArea;
	for (const auto& pair : npcs) {
		auto character = pair.second;
		if (character.getAreaID() == areaID) {
			NPCsInArea.push_back(character.getID());
		}
	}
	return NPCsInArea;
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
	else if (npcs.count(characterID) > 0) {
		return (NPC*)&(npcs.at(characterID));
	}
	return nullptr;
}

NPC* GameModel::getNPCByID(const std::string& npcID) const {
	if (npcs.count(npcID) > 0) {
		return (NPC*)&(npcs.at(npcID));
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
        auto characterID = std::static_pointer_cast<BodySwapStatus>(*statusEffect)->getSwappedID();
		return (Character*)&(characters.at(characterID));
    }
}

void GameModel::addNPCsToAreas() {
    for (const auto& reset : resets) {

        if (reset.getAction() == ACTION_NPC) {
            try {

                auto& npc = npcTemplates.at(reset.getActionID());
                for (int i = 0; i < reset.getLimit(); i++) {
                    std::string newNpcID = npc.getID();
//                    newNpcID = newNpcID.append(std::to_string(npc.getCounter()));
					newNpcID = npc.getName() + "-" + newNpcID.append(std::to_string(npc.getCounter()));

                    auto newNPC = NPC(npc);
                    newNPC.setID(newNpcID);
                    newNPC.setAreaID(reset.getAreaID());
                    npcs.insert(std::pair<std::string, NPC>(newNPC.getID(), newNPC));
                    npc.increaseCounter();
                }

            } catch (std::out_of_range ex) {

            }
        }
    }
}

void GameModel::setNPCs(const std::unordered_map<std::string, NPC> npcs) {
    this->npcTemplates = npcs;
}

void GameModel::setResets(const std::vector<Resets> resets) {
	this->resets = resets;
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

	auto stringInfo = StringInfo{characterID, target, 0, ""};
	c1->pushToBuffer(GameStrings::getFormatted(GameStringKeys::COMBAT_ENGAGE, stringInfo), GameStringKeys::MESSAGE_SENDER_BATTLE, ColorTag::WHITE);
	c2->pushToBuffer(GameStrings::getFormatted(GameStringKeys::COMBAT_ENGAGED, stringInfo), GameStringKeys::MESSAGE_SENDER_BATTLE, ColorTag::WHITE);

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
    manageDeadCharacters();

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
        auto& character = pair.second;
        if (character.getState() == CharacterState::DEAD) {
            // TODO: Add NPC Clause
            character.setAreaID(getDefaultLocationID());
            character.setState(CharacterState::IDLE);
            character.setCurrentHealth(character.getMaxHealth());
            character.setCurrentMana(character.getMaxMana());
            character.getInventory().removeAllItems();
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
		if (eraseIter != statusEffects.end()) {
			character.pushToBuffer(GameStrings::get(GameStringKeys::STATUS_EFFECT_END), GameStringKeys::MESSAGE_SENDER_SERVER, ColorTag::WHITE);
		}
        statusEffects.erase(eraseIter, statusEffects.end());
    }
}

void GameModel::pushToOutputBuffer(const std::string& characterID, std::string message, std::string sender, std::string color) {
	getCharacterByID(characterID)->pushToBuffer(message, sender, color);
}

void GameModel::sendGlobalMessage(const std::string& senderID, std::string message) {
	for (const auto& pair : characters) {
		auto character = pair.second;
		character.pushToBuffer(message, senderID, ColorTag::WHITE);
	}
}

void GameModel::sendLocalMessage(const std::string& senderID, std::string message) {
	auto areaID = getCharacterByID(senderID)->getAreaID();
	for (const auto &character: getCharacterIDsInArea(areaID)) {
		getCharacterByID(character)->pushToBuffer(message, senderID, ColorTag::WHITE);
	}
}

void GameModel::sendPrivateMessage(const std::string& senderID, std::string message, const std::string& target) {
	auto targetCharacter = getCharacterByID(target);
	if (targetCharacter != nullptr) {
		targetCharacter->pushToBuffer(message, senderID, ColorTag::WHITE);
	}
}

void GameModel::listValidSpells(const std::string& characterID) {
	auto character = getCharacterByID(characterID);
	std::string message = GameStrings::get(GameStringKeys::SPELL_LIST) + "\n";
	for (auto& spellPairs: spells) {
		message += spellPairs.first + ", ";
	}
	character->pushToBuffer(message, GameStringKeys::MESSAGE_SENDER_SERVER, ColorTag::WHITE);
}

void GameModel::loadDefaultSpells() {
    Spell bodySwap("body swap", 0, SpellType::BODY_SWAP, "");
    addSpell(bodySwap);
}

void GameModel::castSpell(const std::string& sourceID, const std::string& targetID, const std::string& spellID) {
	auto spellsIter = spells.find(spellID);
	if (spellsIter != spells.end()) {
		if (characterIsInCombat(sourceID)) {
			//TODO: Handle switching targets based on targetID
			setCombatAction(sourceID, spellID);
			return;
		}

		auto& spell = spellsIter->second;
		if (spell.getType() == SpellType::OFFENSE && sourceID != targetID) {
			engageCharacterInCombat(sourceID, targetID);
			setCombatAction(sourceID, spellID);
		}
		else {
			CombatCast spellCast{spell};
			spellCast.execute(*getCharacterByID(sourceID), *getCharacterByID(targetID));
		}
	} else {
		auto unknownSpellMessage = GameStrings::getFormatted(GameStringKeys::SPELL_UNKNOWN, StringInfo{sourceID, targetID, 0, spellID});
		getCharacterByID(sourceID)->pushToBuffer(unknownSpellMessage, GameStringKeys::MESSAGE_SENDER_SERVER, ColorTag::WHITE);
	}
}
