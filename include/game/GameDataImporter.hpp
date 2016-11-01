#ifndef GAME_DATA_IMPORTER_HPP
#define GAME_DATA_IMPORTER_HPP

#include "../../src/game-model/Area.hpp"
#include "../../src/game-model/Entity.hpp"
#include <string>
#include "yaml-cpp/yaml.h"
#include "game/GameModel.hpp"
#include "../../src/game-model/Character.hpp"
#include "../../src/game-model/Resets.h"

class GameDataImporter {

private:

public:


	//Inputs .YAML file for parsing
    static void loadyamlFile(GameModel& gameModel, std::string file);

    static std::vector<NPC> returnNPCS(GameModel& gameModel, YAML::Node);
    static void loadRooms(GameModel& gameModel, YAML::Node);
	static void loadObjects(GameModel& gameModel, YAML::Node);
	static std::vector<Resets> returnResets(GameModel& gameModel, YAML::Node);
	static void loadShops(GameModel& gameModel, YAML::Node);


};

#endif