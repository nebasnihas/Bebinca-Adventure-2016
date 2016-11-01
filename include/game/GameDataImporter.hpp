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

#include "Area.hpp"
#include "Object.hpp"
#include "GameModel.hpp"


class GameDataImporter {

private:

public:

	//Inputs .YAML file for parsing
    static void loadyamlFile(GameModel& gameModel, const std::string& fileName);

    static void loadNPCS(GameModel& gameModel, YAML::Node NPCS);
    static void loadRooms(GameModel& gameModel, YAML::Node ROOMS);
	static void loadObjects(GameModel& gameModel, YAML::Node OBJECTS);
	static void loadResets(GameModel& gameModel, YAML::Node RESETS);
	static void loadShops(GameModel& gameModel, YAML::Node SHOPS);

};

#endif