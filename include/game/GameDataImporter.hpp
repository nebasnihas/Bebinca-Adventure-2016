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
#include <game/GameModel.hpp>
#include <game/Character.hpp>
#include "../../src/game-model/Resets.hpp"

#include <game/Area.hpp>
#include <game/Object.hpp>
#include <game/GameModel.hpp>


class GameDataImporter {

private:

public:


	//Inputs .YAML file for parsing
    static void loadyamlFile(GameModel& gameModel, const std::string& fileName);
	static YAML::Node getRootYAMLNode(GameModel& gameModel, const std::string& fileName);

    static std::vector<Area> getRooms(const YAML::Node& ROOMS);
	static std::vector<Object> getObjects(const YAML::Node& OBJECTS);
	static std::unordered_map<std::string, NPC> returnNPCS(GameModel& gameModel, const YAML::Node&);
	static std::vector<Resets> returnResets(GameModel& gameModel, const YAML::Node&);
	static void loadShops(GameModel& gameModel, const YAML::Node&);


	//Spells
	static void loadSpells(const YAML::Node&);


};

#endif