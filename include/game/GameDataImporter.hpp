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
#include "../../src/game-model/Resets.h"

#include <game/Area.hpp>
#include <game/Object.hpp>
#include <game/GameModel.hpp>


class GameDataImporter {

private:

public:


	//Inputs .YAML file for parsing
    static void loadyamlFile(GameModel& gameModel, const std::string& fileName);

    static std::vector<Area> getRooms(YAML::Node ROOMS);
	static std::vector<Object> getObjects(YAML::Node OBJECTS);
	static std::map<std::string, NPC> returnNPCS(GameModel& gameModel, YAML::Node);
	static std::vector<Resets> returnResets(GameModel& gameModel, YAML::Node);
	static void loadShops(GameModel& gameModel, YAML::Node);


};

#endif