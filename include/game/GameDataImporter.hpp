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
	static std::vector<Object> getObjects(const YAML::Node& OBJECTS);
	static std::unordered_map<std::string, NPC> returnNPCS(const YAML::Node&);
	static std::vector<Resets> returnResets(const YAML::Node&);
	static void loadShops(GameModel& gameModel, const YAML::Node&);


	//Spells
	static std::vector<Spell> getSpells(const YAML::Node&);


};

namespace YAML {
template<>
struct convert<Area> {
	static Node encode(const Area& area) {
		const static std::string ID_KEY = "id";
		const static std::string NAME_KEY = "name";
		const static std::string DESC_KEY = "desc";
		const static std::string EXTENDED_DESC_KEY = "extended_descriptions";
		const static std::string DOORS_KEY = "doors";
		const static std::string DOOR_DIR_KEY = "dir";
		const static std::string DOOR_TO_KEY = "to";

		Node node;
		node[ID_KEY] = area.getID();
		node[NAME_KEY] = area.getTitle();
		node[DESC_KEY].push_back(area.getDescription());
		node[EXTENDED_DESC_KEY] = area.getExtendedDescriptions();
		for (const auto& door : *area.getConnectedAreas()) {
			YAML::Node doorNode;
			doorNode[DOOR_DIR_KEY] = door.first;
			doorNode[DOOR_TO_KEY] = door.second;

			node[DOORS_KEY].push_back(doorNode);
		}

		return node;
	}
};
}

#endif