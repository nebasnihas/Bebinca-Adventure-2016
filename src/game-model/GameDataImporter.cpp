#include "../../include/game/GameDataImporter.hpp"

using namespace std;

using std::vector;
using std::string;
using std::unordered_map;



//NOTE: If compiling in command line, must include -lyaml-cpp flag at the end of g++ sequence

void GameDataImporter::loadyamlFile(GameModel& gameModel, const std::string& fileName) {
	//Loading source .yaml file,split at initial nodes (NPCS, ROOM, OBJECTS, RESETS, SHOPS)

    YAML::Node dataFile = YAML::LoadFile(fileName);
	//const YAML::Node NPCS = dataFile["NPCS"];
    //loadNPCS(gameModel, NPCS);

    //const YAML::Node ROOMS = dataFile["ROOMS"];
    //loadRooms(gameModel, ROOMS);

    const YAML::Node OBJECTS = dataFile["OBJECTS"];
    loadObjects(/*gameModel,*/ OBJECTS);

    //const YAML::Node RESETS = dataFile["RESETS"];
    //loadResets(gameModel, RESETS);

    //const YAML::Node SHOPS = dataFile["SHOPS"];
    //loadShops(gameModel, SHOPS);

}

//The following five methods take a root node and parse it one level deeper (to get an individual description)

void GameDataImporter::loadNPCS(GameModel& gameModel, YAML::Node NPCS){
    //Sequence Iterator
    for(YAML::const_iterator it = NPCS.begin(); it != NPCS.end(); ++it){

        const YAML::Node NPC = *it;

        string armor = NPC["armor"].as<string>();
        string damage = NPC["damage"].as<string>();
        vector<string> description = NPC["description"].as<vector<string>>();
        int exp =  NPC["exp"].as<int>();
        int gold = NPC["gold"].as<int>();
        string hit = NPC["hit"].as<std::string>();
        string npcID = NPC["id"].as<string>();
        vector<string> keywords = NPC["keywords"].as<vector<string>>();
        int level = NPC["level"].as<int>();
        vector<string> longdesc = NPC["longdesc"].as<vector<string>>();
        string shortdesc = NPC["shortdesc"].as<string>();
        int thac0 = NPC["thac0"].as<int>();

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

    for(YAML::const_iterator it = ROOMS.begin(); it != ROOMS.end(); ++it){

        const YAML::Node ROOM = *it;

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
        for(YAML::const_iterator it = doors.begin(); it != doors.end(); ++it){

            const YAML::Node door = *it;

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
    //gameModel.setDefaultLocationID(rooms[0].getID());
    // for (const auto& room : rooms) {
    //     gameModel.addArea(room);
    // }
    cout << rooms.size();

}

void GameDataImporter::loadObjects(/*GameModel& gameModel,*/ YAML::Node OBJECTS){

    vector<Object> objects = {};

    /*
    Split up objects in YML file and store them in the objects class
    */

    for(YAML::const_iterator it = OBJECTS.begin(); it != OBJECTS.end(); ++it){

        const YAML::Node OBJECT = *it;

        vector<string> attributes = OBJECT["attributes"].as<vector<string>>();


        //COST
        int cost = OBJECT["cost"].as<int>();

        vector<string> extra = OBJECT["extra"].as<vector<string>>();
        string objectId = OBJECT["id"].as<string>();
        string item_type = OBJECT["item_type"].as<string>();
        vector<string> keywords = OBJECT["keywords"].as<vector<string>>();
        
        vector<string> longdesc = OBJECT["longdesc"].as<vector<string>>();
        string description = boost::algorithm::join(longdesc, " ");

        //cout << description << endl;

        string shortdesc = OBJECT["shortdesc"].as<string>();
        vector<string> wear_flags = OBJECT["wear_flags"].as<vector<string>>();
        int weight = OBJECT["weight"].as<int>();

        //Create objects of type Object and store in vector ob Objects
        Object newObject = Object(attributes, cost, extra, objectId, item_type, keywords, description, shortdesc, wear_flags, weight);
        objects.push_back(newObject);
    }

    cout << "Number of objects in Mother goose area: " << objects.size() << endl;

    for (auto object : objects)
    {
        cout << "Cost of Mother Goose objects: " << object.getCost() << endl;
        cout << "Description: " << object.getDescription() << endl;
        cout << "Type: " << object.getType() << endl;
        cout << "ID: " << object.getID() << endl;
    }

}

//The workflow for RESETS ends here, not sure how to utilize yet
void GameDataImporter::loadResets(GameModel& gameModel, YAML::Node RESETS){

    for(YAML::const_iterator it = RESETS.begin(); it != RESETS.end(); ++it){

        const YAML::Node RESET = *it;

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

    for(YAML::const_iterator it = SHOPS.begin(); it != SHOPS.end(); ++it){

        const YAML::Node SHOP = *it;
        //no data on shops in mgoose file

    }

}



//use main for testing
//int main() {
//
//    GameModel gameModel = GameModel();
//	GameDataImporter::loadyamlFile(gameModel, "../../data/mgoose.yml");
//	return 0;
//}