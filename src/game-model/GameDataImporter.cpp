#include "GameDataImporter.hpp"
#include "yaml-cpp/yaml.h"
#include <fstream>
#include <iostream>

#include <string>

//NOTE: If compiling in command line, must include -lyaml-cpp flag at the end of g++ sequence

void GameDataImporter::loadyamlFile(std::string fileName) {
	//Loading source .yaml file,split at initial nodes (NPCS, ROOM, OBJECTS, RESETS, SHOPS)

    YAML::Node dataFile = YAML::LoadFile(fileName);
	const YAML::Node NPCS = dataFile["NPCS"];
        loadNPCS(NPCS);

}

//The following five methods take a root node and parse it one level deeper (to get an individual description)

void GameDataImporter::loadNPCS(YAML::Node NPCS){
    //Sequence Iterator
    for(YAML::const_iterator it = NPCS.begin(); it != NPCS.end(); ++it){

        const YAML::Node NPC = *it;

        std::cout << "armor: " << NPC["armor"].as<std::string>() << std::endl;
        std::cout << "damage: " << NPC["damage"].as<std::string>() << std::endl;
        std::cout << "description: " << NPC["description"] << std::endl;
        std::cout << "exp: " << NPC["exp"].as<int>() << std::endl;
        std::cout << "gold: " << NPC["gold"].as<int>() << std::endl;
        std::cout << "hit: " << NPC["hit"].as<std::string>() << std::endl;
        std::cout << "id: " << NPC["id"].as<int>() << std::endl;
        std::cout << "keywords: " << NPC["keywords"] << std::endl;
        std::cout << "level: " << NPC["level"].as<int>() << std::endl;
        std::cout << "longdesc: " << NPC["longdesc"] << std::endl;
        std::cout << "shortdesc: " << NPC["shortdesc"].as<std::string>() << std::endl;
        std::cout << "thac0: " << NPC["thac0"].as<int>() << std::endl;

    }
}
//void GameDataImporter::loadRooms(YAML::Node ROOMS);
//void GameDataImporter::loadObjects(YAML::Node OBJECTS);
//void GameDataImporter::loadResets(YAML::Node RESETS); //The workflow for RESETS ends here, not sure how to utilize yet
//void GameDataImporter::loadShops(YAML::Node SHOPS); //The workflow for SHOPS ends here, not sure how to utilize yet

//Takes a single object desription, creates an object using the entity constructor, pushes object onto a vector of entities
void loadSingleObjects(YAML::Node);

//Takes a single room description, creates an object using the area constructor, pushes object onto a vector of entities
void loadSingleRoom(YAML::Node);


//use main for testing
int main() {


	GameDataImporter::loadyamlFile("../../data/mgoose.yml");



	return 0;
}