#include "GameDataImporter.hpp"


GameDataImporter::loadyamlFile(std::string fileName) {
	//Loading source .yaml file,split at initial nodes (ROOM, OBJECTS, RESETS, SHOPS)
	YAML::Node config = YAML::LoadFile(fileName);

	//pass each of the 4 root nodes to the correct method
}

//The following four methods take a root node, parse it one level deeper (to get an individual description), 
//and pass it to the correct private method
void loadRooms(YAML::Node);
void loadObjects(YAML::Node);
void loadResets(YAML::Node); //The workflow for RESETS ends here, not sure how to utilize yet
void loadShops(YAML::Node); //The workflow for SHOPS ends here, not sure how to utilize yet

//Takes a single object desription, creates an object using the entity constructor, pushes object onto a vector of entities
void loadSingleObjects(YAML::Node);

//Takes a single room description, creates an object using the area constructor, pushes object onto a vector of entities
void loadSingleRoom(YAML::Node);



int main() {


	loadymlFile("../../data/mgoose.yml");



	return 0;
}