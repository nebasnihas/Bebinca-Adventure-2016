#include "game/GameDataImporter.hpp"
#include "Area.hpp"
#include "Character.hpp"
#include "yaml-cpp/yaml.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <boost/algorithm/string/join.hpp>

using namespace std;

using std::vector;
using std::string;
using std::unordered_map;

#include <string>


//NOTE: If compiling in command line, must include -lyaml-cpp flag at the end of g++ sequence

void GameDataImporter::loadyamlFile(GameModel& gameModel, std::string fileName) {
	//Loading source .yaml file,split at initial nodes (NPCS, ROOM, OBJECTS, RESETS, SHOPS)

    YAML::Node dataFile = YAML::LoadFile(fileName);
	const YAML::Node NPCS = dataFile["NPCS"];
    loadNPCS(gameModel, NPCS);

    const YAML::Node ROOMS = dataFile["ROOMS"];
    loadRooms(gameModel, ROOMS);

    const YAML::Node OBJECTS = dataFile["OBJECTS"];
    loadObjects(gameModel, OBJECTS);

    const YAML::Node RESETS = dataFile["RESETS"];
    loadResets(gameModel, RESETS);

    const YAML::Node SHOPS = dataFile["SHOPS"];
    loadShops(gameModel, SHOPS);

}

//The following five methods take a root node and parse it one level deeper (to get an individual description)

void GameDataImporter::loadNPCS(GameModel& gameModel, YAML::Node NPCS){

    vector<NPC> npcs = {};

    //Sequence Iterator
    for(YAML::Node NPC : NPCS){

        int armor = NPC["armor"].as<int>();
        string damage = NPC["damage"].as<string>();
        string shortdesc = NPC["shortdesc"].as<string>();
        int thac0 = NPC["thac0"].as<int>();
        int exp =  NPC["exp"].as<int>();
        int gold = NPC["gold"].as<int>();
        string hit = NPC["hit"].as<string>();
        string npcID = NPC["id"].as<string>();
        int level = NPC["level"].as<int>();

        vector<string> description = NPC["description"].as<vector<string>>();
        string sDescription = boost::algorithm::join(description, " ");

        vector<string> keywords = NPC["keywords"].as<vector<string>>();
        string sKeywords = boost::algorithm::join(keywords, " ");

        vector<string> longdesc = NPC["longdesc"].as<vector<string>>();
        string sLongDescription = boost::algorithm::join(longdesc, " ");

        //Create NPC
        NPC newNPC = NPC::NPC(npcID, shortdesc, hit, damage, level, exp, armor, thac0, gold, sDescription, sKeywords, sLongDescription);
        npcs.push_back(newNPC);
    }
}

void GameDataImporter::loadRooms(GameModel& gameModel, YAML::Node ROOMS){

    //Create vector to hold instances of ROOM
    vector<Area> rooms;
    std::string roomDescription;

   	/*
   	Room objects contain:

   	string ID
   	string roomName
   	map of connected areas (string, string)
   	string description (of area)

	*/

    for(YAML::Node ROOM : ROOMS){

        //Store room descriptions
        vector<string> desc = ROOM["desc"].as<vector<string>>();
        string description = boost::algorithm::join(desc, " ");

        //Store ID and name of room
        string id = ROOM["id"].as<string>();
        string name = ROOM["name"].as<string>();

        //Area newArea = Area(id, name);
        //rooms.push_back(newArea);

        //cout << "ID: \t" << newArea.getID() << "Name: " << newArea.getTitle() << endl;



        //dir is key, maps to room ID
        unordered_map<string, string> doorsMap;
        const YAML::Node doors = ROOM["doors"];
        for(YAML::Node door : doors){

            string dir = door["dir"].as<string>();
            string to = door["to"].as<string>();

            //Following two are currently empty on the YAML file
//            vector<string> desc = door["desc"].as<vector<string>>();
//            vector<string> doorKeywords = door["keywords"].as<vector<string>>();

            std::pair<string, string> doorKeyValuePair (dir, to);
            doorsMap.insert(doorKeyValuePair);
        }

        //Create an Area objects with ID, title, connected areas, and descriptions
        Area newArea = Area(id, name, doorsMap, description);
        rooms.push_back(newArea);

        cout << "ID: " << newArea.getID() << endl << "Name: " << newArea.getTitle() << endl;


		//Print out contents of map
		for (const auto &pair : *newArea.getConnectedAreas())
		{
    		cout << "m[" << pair.first << "] = " << pair.second << '\n';
		}

		//Print out description of each Area
		cout << newArea.getDescription() << endl;

    }

    //Might want to return the vector of Area objects
    gameModel.setDefaultLocationID(rooms[0].getID());
    for (const auto& room : rooms) {
        gameModel.addArea(room);
    }
    cout << rooms.size();

}

void GameDataImporter::loadObjects(GameModel& gameModel, YAML::Node OBJECTS){

    for(YAML::Node OBJECT : OBJECTS){

        vector<string> attributes = OBJECT["attributes"].as<vector<string>>();
        int cost = OBJECT["cost"].as<int>();
        vector<string> extra = OBJECT["extra"].as<vector<string>>();
        string objectId = OBJECT["id"].as<string>();
        string item_type = OBJECT["item_type"].as<string>();
        vector<string> longdesc = OBJECT["longdesc"].as<vector<string>>();
        string shortdesc = OBJECT["shortdesc"].as<string>();
        vector<string> wear_flags = OBJECT["wear_flags"].as<vector<string>>();
        int weight = OBJECT["weight"].as<int>();

    }
}

//The workflow for RESETS ends here, not sure how to utilize yet
void GameDataImporter::loadResets(GameModel& gameModel, YAML::Node RESETS){

    for(YAML::Node RESET : RESETS){

        //ie. does it reset npc, equip, door etc.
        string resetAction = RESET["action"].as<string>();
        string itemId = RESET["id"].as<string>();

        //Different reset actions have different properties, these must be taken into account
        if(resetAction == "npc") int limit = RESET["limit"].as<int>();
        if(resetAction != "equip") string roomId = RESET["room"].as<string>();
        if(resetAction == "equip") int slot = RESET["slot"].as<int>();
        if(resetAction == "door") string stateOfDoor = RESET["state"].as<string>();

    }
}

//The workflow for SHOPS ends here, not sure how to utilize yet
void GameDataImporter::loadShops(GameModel& gameModel, YAML::Node SHOPS){

    for(YAML::Node SHOP : SHOPS){
        //no data on shops in mgoose file

    }

}



//use main for testing
/*int main() {


	GameDataImporter::loadyamlFile("../../data/mgoose.yml");



	return 0;
}*/