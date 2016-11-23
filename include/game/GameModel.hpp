#ifndef GAME_MODEL_HPP
#define GAME_MODEL_HPP

#include <unordered_map>
#include <algorithm>
#include <utility>
#include <exception>
#include <deque>
#include <memory>

#include <game/Area.hpp>
#include <game/Character.hpp>
#include "../../src/game-model/Resets.hpp"
#include "Object.hpp"
#include <game/Inventory.hpp>
#include "../../src/game-model/combat/CombatManager.hpp"

class GameModel {

public:

	/*
	 *	CHARACTER FUNCTIONS
	 */

    // TODO: Make this configurable
    static const int GAME_TICKS_PER_COMBAT_TICK = 30;

	GameModel();

	bool createCharacter(const std::string& characterID, const std::string& characterName);
	bool moveCharacter(const std::string& characterID, const std::string& areaTag);
	Character* getCharacterByID(const std::string& characterID) const;

	/*
	 *	OBJECT FUNCTIONS
	 */

	std::string getObjectDescription(const std::string& areaID, const std::string& objectName) const;

	/*
	 *	AREA FUNCTIONS
	 */

  	Area* getAreaByID(const std::string& areaID) const;
  	std::string getAreaDescription(const std::string& areaID) const;
  	std::unordered_map<std::string, std::string>* getConnectedAreas(const std::string& areaID) const;
	std::vector<std::string> getCharacterIDsInArea(const std::string& areaID) const;

	/*
	 *	LOCATION FUNCTIONS
	 *	TO-DO: Make the location an actual area.
	 */

	std::string getDefaultLocationID() const;
	void setDefaultLocationID(const std::string& locationID);



    void createNPC(const std::string& npcID,
                   const std::string& name, //Name is the short desc
                   std::string& hit,
                   std::string& damage,
                   int level,
                   int exp,
                   int armor,
                   int gold,
                   Inventory inventory,
                   std::string& areaID,
                   int thac0,
                   const std::string& description,
                   const std::string& keywords,
                   const std::string& longDesc);

    void setNPCs(const std::unordered_map<std::string, NPC> npcs);
    void addNPCsToAreas();
    bool addArea(const Area area);
    bool characterIsInCombat(const std::string& characterID);
    bool characterIsDead(const std::string& characterID);

	std::string getEntityDescription(const std::string& areaID, const std::string& entityID) const;

    void loadActions(const std::unordered_map<std::string, std::shared_ptr<CombatAction>>& actionLookup);
	void addSpell(Spell spell);

    bool engageCharacterInCombat(const std::string& characterID, const std::string& target);
    bool setCombatAction(const std::string& characterID, const std::string& actionName);
    bool setCombatTarget(const std::string& characterID, const std::string& targetID);
    std::vector<std::string> getPossibleTargets(const std::string& characterID);
    std::vector<std::string> getAvailableActions(const std::string& characterID);
	void listValidSpells(const std::string& characterID);
    void update();
	void castSpell(const std::string& sourceID, const std::string& targetID, const std::string& spellID);

	void pushToOutputBuffer(const std::string& characterID, std::string message, std::string sender, int color);

private:
    void manageDeadCharacters();
    bool characterCanMove(const Character& character);
    Character* getBodySwappedCharacter(Character* character) const;
	void updateStatusEffects();
	void loadDefaultSpells();

	CombatManager combatManager;
    std::unordered_map<std::string, Character> characters;
	std::unordered_map<std::string, Area> locations;
	std::unordered_map<std::string, NPC> npcs;
    std::vector<Resets> resets;

	std::string defaultLocation;
	std::unordered_map<std::string, Spell> spells;

    std::unordered_map<std::string, NPC> npcTemplates;

    unsigned long long gameTicks = 0;

}; //GameModel class


/*
 *  VARIOUS GAME MODEL EXCEPTIONS
 */

struct InvalidMoveException : public std::exception
{

    virtual const char* what() const throw()
    {
        return "This move is invalid.";
    }

}; //Invalid character exception

#endif