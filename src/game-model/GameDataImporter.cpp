#include "GameDataImporter.hpp"


GameDataImporter::loadymlFile(std::string fileName) {

	//Loading source .yaml file
	YAML::Node config = YAML::LoadFile(fileName);

}



int main() {

	loadymlFile("../../data/mgoose.yml");



	return 0;
}