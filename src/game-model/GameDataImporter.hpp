#ifndef GAME_DATA_IMPORTER_HPP
#define GAME_DATA_IMPORTER_HPP

#include "Area.hpp"
#include <string>
#include "yaml-cpp/yaml.h"


class GameDataImporter {

private:

	std::string datafile;
	static void loadSingleRoom(YAML::Node);
	static void loadSingleObjects(YAML::Node);
	//void loadSingleResets(YAML::Node);
	//void loadSingleShops(YAML::Node);


public:

	//Inputs .YAML file for parsing
    static void loadyamlFile(std::string file);

    static void loadNPCS(YAML::Node);
    static void loadRooms(YAML::Node);
	static void loadObjects(YAML::Node);
	static void loadResets(YAML::Node);
	static void loadShops(YAML::Node);


};

#endif