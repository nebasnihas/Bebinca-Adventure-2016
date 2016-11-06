#include "game/GameDataImporter.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::unordered_map;


//Helper functions
//string getStringData(const YAML::Node node, string keyword);



//NOTE: If compiling in command line, must include -lyaml-cpp flag at the end of g++ sequence

void GameDataImporter::loadyamlFile(GameModel& gameModel, const std::string& fileName) {
	//Loading source .yaml file,split at initial nodes (NPCS, ROOM, OBJECTS, RESETS, SHOPS)

    YAML::Node dataFile = YAML::LoadFile(fileName);
	const YAML::Node NPCS = dataFile["NPCS"];
    returnNPCS(gameModel, NPCS);

    const YAML::Node ROOMS = dataFile["ROOMS"];
    getRooms(ROOMS);

    const YAML::Node OBJECTS = dataFile["OBJECTS"];
    getObjects(OBJECTS);

    const YAML::Node RESETS = dataFile["RESETS"];
    returnResets(gameModel, RESETS);

    const YAML::Node SHOPS = dataFile["SHOPS"];
    //loadShops(/*gameModel,*/ SHOPS);


    //SPELLS
    const YAML::Node DEFENSE_SPELLS = dataFile["defense"];
    const YAML::Node OFFENSE_SPELLS = dataFile["offense"];
    loadSpells(/*gameModel,*/ DEFENSE_SPELLS);

}

//The following five methods take a root node and parse it one level deeper (to get an individual description)

std::map<std::string, NPC> GameDataImporter::returnNPCS(GameModel& gameModel, YAML::Node NPCS){

    std::map<std::string, NPC> npcs;

    //Sequence Iterator
    for(YAML::Node nodeNPC : NPCS){

        int armor = nodeNPC["armor"]?
                    nodeNPC["armor"].as<int>()
                    :NPC::defaultArmor;


        string damage = nodeNPC["damage"]?
                        nodeNPC["damage"].as<string>()
                        :NPC::defaultDamage;

        string shortdesc = nodeNPC["shortdesc"]?
                            nodeNPC["shortdesc"].as<string>()
                            :NPC::defaultName;

        int thac0 = nodeNPC["thac0"]?
                    nodeNPC["thac0"].as<int>()
                    :NPC::defaultThac0;

        int exp =  nodeNPC["exp"]?
                    nodeNPC["exp"].as<int>()
                    :NPC::defaultExp;

        int gold = nodeNPC["gold"]?
                    nodeNPC["gold"].as<int>()
                    :NPC::defaultGold;

        string hit = nodeNPC["hit"]?
                     nodeNPC["hit"].as<string>()
                     :NPC::defaultHit;

        string npcID = nodeNPC["id"]?
                        nodeNPC["id"].as<string>()
                        :NPC::defaultID;

        int level = nodeNPC["level"]?
                    nodeNPC["level"].as<int>()
                    :NPC::defaultLevel;

        vector<string> description = nodeNPC["description"]?
                                     nodeNPC["description"].as<vector<string>>()
                                     :NPC::defaultDescription;
                    string sDescription = boost::algorithm::join(description, " ");


        vector<string> keywords = nodeNPC["keywords"]?
                                 nodeNPC["keywords"].as<vector<string>>()
                                 :NPC::defaultKeywords;
                    string sKeywords = boost::algorithm::join(keywords, " ");

        vector<string> longdesc = nodeNPC["longdesc"]?
                                    nodeNPC["longdesc"].as<vector<string>>()
                                            :NPC::defaultLongDescription;
            string sLongDescription = boost::algorithm::join(longdesc, " ");

        Inventory inventory;

        std::string areaID;

        //Create NPC instance
        NPC newNPC = NPC(npcID, shortdesc, hit, damage, level, exp, armor, gold, inventory, areaID, thac0,
                              sDescription, sKeywords, sLongDescription);
        npcs.insert(std::pair<std::string, ::NPC>(npcID, newNPC));
    }

    return npcs;
}

std::vector<Area> GameDataImporter::getRooms(YAML::Node ROOMS){

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

        //Store extended room descriptions
        vector<string> extended_descriptions = ROOM["extended_descriptions"].as<vector<string>>();

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
        Area newArea = Area(id, name, doorsMap, description, extended_descriptions);
        rooms.push_back(newArea);

        cout << "ID: " << newArea.getID() << endl << "Name: " << newArea.getTitle() << endl;


		//Print out contents of map
		for (const auto &pair : *newArea.getConnectedAreas())
		{
    		cout << "m[" << pair.first << "] = " << pair.second << '\n';
		}

		//Print out description of each Area
		cout << newArea.getDescription() << endl;

        for (const auto &ext_desc : newArea.getExtendedDescriptions())
        {
            cout << "Extended Description: " <<  ext_desc << "\n";
        }

    }

    //Might want to return the vector of Area objects
    //gameModel.setDefaultLocationID(rooms[0].getID());
    // for (const auto& room : rooms) {
    //     gameModel.addArea(room);
    // }
    cout << rooms.size();

    return rooms;

}

std::vector<Object> GameDataImporter::getObjects(YAML::Node OBJECTS){

    vector<Object> objects = {};

    /*
    Split up objects in YML file and store them in the objects class
    */

    for(YAML::Node OBJECT : OBJECTS){

        vector<string> attributes = OBJECT["attributes"].as<vector<string>>();


        //COST
        int cost = OBJECT["cost"].as<int>();

        vector<string> extra = OBJECT["extra"].as<vector<string>>();
        string objectId = OBJECT["id"].as<string>();
        string item_type = OBJECT["item_type"].as<string>();
        vector<string> keywords = OBJECT["keywords"].as<vector<string>>();

        vector<string> longdesc = OBJECT["longdesc"].as<vector<string>>();
        string description = boost::algorithm::join(longdesc, " ");

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


    return objects;

}

//The workflow for RESETS ends here, not sure how to utilize yet
vector<Resets> GameDataImporter::returnResets(GameModel& gameModel, YAML::Node RESETS){

    vector<Resets> resets = {};

    for(YAML::Node RESET : RESETS){

        //ie. does it reset npc, equip, door etc.
        string resetAction = " ";
        if(RESET["action"]){
            resetAction = RESET["action"].as<string>();

        }

        string itemID = " ";
        if(RESET["id"]){
            itemID = RESET["id"].as<string>();
        }

        string roomID = " ";
        if(RESET["room"]){
            roomID = RESET["room"].as<string>();
        }

        string stateOfDoor = " ";
        if(RESET["state"]){
            stateOfDoor = RESET["state"].as<string>();
        }

        int limit = 1;
        if(RESET["limit"]){
            limit = RESET["limit"].as<int>();
        }

        int slot = 0;
        if(RESET["slot"]){
            slot = RESET["slot"].as<int>();
        }

    Resets newReset = Resets(resetAction, itemID, roomID, stateOfDoor, slot, limit);

    resets.push_back(newReset);

    }

    return resets;
}


//Spells

void GameDataImporter::loadSpells(YAML::Node DEFENSE_SPELLS) {

    //Defense top level node
    for (YAML::Node DEFENSE : DEFENSE_SPELLS) {

        int duration = 0;
        string effect;
        string wearoff;
        string hitchar;
        string hitroom;
        string hitvict;
        string dammsg;
        //vector<string> wearoffVector;

        if (DEFENSE["Wearoff"]) {
            wearoff = DEFENSE["Wearoff"].as<string>();
        }

        if (DEFENSE["Dammsg"]) {
            dammsg = DEFENSE["Wearoff"].as<string>();
        }

        //wearoff = getStringData(DEFENSE, "wearoff");

        if (DEFENSE["Hitchar"]) {
            hitchar = DEFENSE["Hitchar"].as<string>();
        }

        if (DEFENSE["Hitroom"]) {
            hitroom = DEFENSE["Hitroom"].as<string>();
        }

        if (DEFENSE["Hitvict"]) {
            hitvict = DEFENSE["Hitvict"].as<string>();
        }




        if (DEFENSE["Effect"]) {
            //vector<string> EffectsVector = DEFENSE["Effect"].as<vector<string>>();
            //effects = boost::algorithm::join(EffectsVector, " ");
            effect = DEFENSE["Effect"].as<string>();
        }

        if (DEFENSE["Duration"]) {
            duration = DEFENSE["Duration"].as<int>();
        }

        int mana = DEFENSE["Mana"].as<int>();
        string name = DEFENSE["Name"].as<string>();
        int minLevel = DEFENSE["Minlevel"].as<int>();




        cout << "Name: " << name << endl;
        cout << "Effect: " << effect << endl;
        cout << "Mana: " << mana << endl;
        cout << "Duration: " << duration << endl;
        cout << "Min Level: " << minLevel << endl;
        cout << "Wearoff: " << wearoff << endl;
        cout << "Hitchar: " << hitchar << endl;
        cout << "Hitroom: " << hitroom << endl;
        cout << "Hitvict: " << hitvict << endl;
        //cout << "Dammsg: " << dammsg << endl;

        cout << "*************" << endl;


    }

//    for (YAML::Node DEFENSE : DEFENSES) {
//
//
//        //string effects = DEFENSE["Effect"].as<string>();
//        int mana = DEFENSE["Mana"].as<int>();
//        string name = DEFENSE["Name"].as<string>();
//        int minLevel = DEFENSE["Minlevel"].as<int>();
//        //double duration = DEFENSE["Duration"].as<double>();
//
//        cout << "Name: " << name << endl;
//        //cout << "Effect: " << effects << endl;
//        cout << "Mana: " << mana << endl;
//        //cout << "Duration: " << duration << endl;
//        cout << "Min Level: " << minLevel << endl;
//
//        cout << "*************" << endl;
//
//
//    }


}

//Spell helper functions
string getStringData(const YAML::Node node, string keyword) {
    if (node[keyword]) {
        //vector<string> EffectsVector = DEFENSE["Effect"].as<vector<string>>();
        //effects = boost::algorithm::join(EffectsVector, " ");
        return node[keyword].as<string>();
    }
}



//The workflow for SHOPS ends here, not sure how to utilize yet
void GameDataImporter::loadShops(GameModel& gameModel, YAML::Node SHOPS){

    for(YAML::Node SHOP : SHOPS){
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