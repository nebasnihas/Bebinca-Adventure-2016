#ifndef GAME_MODEL_HPP
#define GAME_MODEL_HPP

#include <map>
#include <algorithm>
#include <utility>

#include "../../src/game-model/Area.hpp"
#include "../../src/game-model/Character.hpp"
#include "../../src/game-model/combat/CombatManager.hpp"

class GameModel {

public:

	bool createCharacter(const std::string& characterID, const std::string& characterName);
	bool moveCharacter(const std::string& characterID, const std::string& areaTag);
	Character* getCharacterByID(const std::string& characterID) const;

    bool addArea(const Area area);
	std::string getDefaultLocationID() const;
	void setDefaultLocationID(const std::string& locationID);
    bool characterCanMove(const Character& character);

	Area* getAreaByID(const std::string& areaID) const;
	std::vector<std::string> getCharacterIDsInArea(const std::string& areaID) const;
    std::unordered_map<std::string, std::string>* getConnectedAreas(const std::string& areaID) const;
	std::string getAreaDescription(const std::string& areaID) const;

	std::string getEntityDescription(const std::string& areaID, const std::string& entityID) const;

	bool engageCharacterInCombat(const std::string& characterID, const std::string& target);
	bool setCombatAction(const std::string& characterID, const std::string& actionName);
	bool setCombatTarget(const std::string& characterID, const std::string& targetID);
    std::vector<std::string> getPossibleTargets(const std::string& characterID);
    std::vector<std::string> getAvailableActions(const std::string& characterID);

    void update();
private:

    void manageDeadCharacters();

	CombatManager combatManager;
	std::map<std::string, Character> characters;
	std::map<std::string, Area> locations;
	std::string defaultLocation;
};

#endif