#ifndef GAME_DATA_IMPORTER_HPP
#define GAME_DATA_IMPORTER_HPP

#include "Area.hpp"
#include <string>
#include "../../lib/parsers/yaml-cpp/include/yaml-cpp/yaml.h"


class GameDataImporter {

private:

	std::string datafile;
	void loadSingleRoom(YAML::Node);
	void loadSingleObjects(YAML::Node);
	//void loadSingleResets(YAML::Node);
	//void loadSingleShops(YAML::Node);


public:

	//Inputs .YAML file for parsing
	void loadyamlFile(std::string file);

	void loadRooms(YAML::Node);
	void loadObjects(YAML::Node);
	void loadResets(YAML::Node);
	void loadShops(YAML::Node);


};

#endif