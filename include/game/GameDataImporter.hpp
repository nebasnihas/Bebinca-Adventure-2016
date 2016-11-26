#ifndef GAME_DATA_IMPORTER_HPP
#define GAME_DATA_IMPORTER_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <boost/algorithm/string/join.hpp>

#include "yaml-cpp/yaml.h"
#include <game/Character.hpp>
#include "../../src/game-model/Resets.hpp"

#include <game/Area.hpp>
#include <game/Spell.hpp>
#include <game/Object.hpp>
#include <game/GameModel.hpp>
#include <game/NPCScripts.hpp>

class GameDataImporter {

private:

	static bool tryParseSpell(const YAML::Node& SPELL, SpellType spellType, Spell& retSpell);

public:


	//Inputs .YAML file for parsing
    static YAML::Node getRootYAMLNode(const std::string& fileName);

    static std::vector<Area> getRooms(const YAML::Node& ROOMS);
	static std::unordered_map<std::string, Object> returnObjects(const YAML::Node& OBJECTS);
	static std::unordered_map<std::string, NPC> returnNPCS(const YAML::Node&);
	static std::vector<Resets> returnResets(const YAML::Node&);
	//static void loadShops(GameModel& gameModel, const YAML::Node&);


	//Spells
	static std::vector<Spell> getSpells(const YAML::Node&);


};

#endif