#include "game/GameDataImporter.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::unordered_map;

YAML::Node GameDataImporter::getRootYAMLNode(GameModel& gameModel, const std::string& fileName) {
    return YAML::LoadFile(fileName);;
}

std::unordered_map<std::string, NPC> GameDataImporter::returnNPCS(GameModel& gameModel, const YAML::Node& NPCS){
    std::unordered_map<std::string, NPC> npcs;

    for(const auto& nodeNPC : NPCS){
        int armor = nodeNPC["armor"]? nodeNPC["armor"].as<int>() :NPC::defaultArmor;
        int thac0 = nodeNPC["thac0"] ? nodeNPC["thac0"].as<int>() : NPC::defaultThac0;
        int exp =  nodeNPC["exp"] ? nodeNPC["exp"].as<int>() : NPC::defaultExp;
        int gold = nodeNPC["gold"] ? nodeNPC["gold"].as<int>() : NPC::defaultGold;
        int level = nodeNPC["level"] ? nodeNPC["level"].as<int>() : NPC::defaultLevel;

        string damage = nodeNPC["damage"] ? nodeNPC["damage"].as<string>() : NPC::defaultDamage;
        string shortdesc = nodeNPC["shortdesc"] ? nodeNPC["shortdesc"].as<string>() : NPC::defaultName;
        string hit = nodeNPC["hit"] ? nodeNPC["hit"].as<string>() : NPC::defaultHit;
        string npcID = nodeNPC["id"] ? nodeNPC["id"].as<string>() : NPC::defaultID;

        vector<string> description = nodeNPC["description"] ? nodeNPC["description"].as<vector<string>>() : NPC::defaultDescription;
        vector<string> keywords = nodeNPC["keywords"] ? nodeNPC["keywords"].as<vector<string>>() : NPC::defaultKeywords;
        vector<string> longdesc = nodeNPC["longdesc"] ? nodeNPC["longdesc"].as<vector<string>>() : NPC::defaultLongDescription;

        string sDescription = boost::algorithm::join(description, " ");
        string sKeywords = boost::algorithm::join(keywords, " ");
        string sLongDescription = boost::algorithm::join(longdesc, " ");

        Inventory inventory;
        std::string areaID;

        NPC newNPC = NPC(npcID, shortdesc, hit, damage, level, exp, armor, gold, inventory, areaID, thac0,
                              sDescription, sKeywords, sLongDescription);
        npcs.insert(std::pair<std::string, ::NPC>(npcID, newNPC));
    }
    return npcs;
}


/*
Room objects contain:
string ID
string roomName
map of connected areas (string, string)
string description (of area)
*/

std::vector<Area> GameDataImporter::getRooms(const YAML::Node& ROOMS) {
    vector<Area> rooms;
    std::string roomDescription;

    for(const auto& ROOM : ROOMS){
        vector<string> desc = ROOM["desc"].as<vector<string>>();
        string description = boost::algorithm::join(desc, " ");
        vector<string> extended_descriptions = ROOM["extended_descriptions"].as<vector<string>>();
        string id = ROOM["id"].as<string>();
        string name = ROOM["name"].as<string>();

        unordered_map<string, string> doorsMap;
        const YAML::Node doors = ROOM["doors"];
        for(const auto& door : doors){
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
    }
    return rooms;
}

std::vector<Object> GameDataImporter::getObjects(const YAML::Node& OBJECTS){
    vector<Object> objects = {};

    /*
    Split up objects in YML file and store them in the objects class
    */

    for(const auto& OBJECT : OBJECTS){
        int cost = OBJECT["cost"].as<int>();
        int weight = OBJECT["weight"].as<int>();

        string objectId = OBJECT["id"].as<string>();
        string item_type = OBJECT["item_type"].as<string>();
        string shortdesc = OBJECT["shortdesc"].as<string>();

        vector<string> attributes = OBJECT["attributes"].as<vector<string>>();
        vector<string> extra = OBJECT["extra"].as<vector<string>>();
        vector<string> keywords = OBJECT["keywords"].as<vector<string>>();
        vector<string> longdesc = OBJECT["longdesc"].as<vector<string>>();
        vector<string> wear_flags = OBJECT["wear_flags"].as<vector<string>>();

        string description = boost::algorithm::join(longdesc, " ");

        Object newObject = Object(attributes, cost, extra, objectId, item_type, keywords, description, shortdesc, wear_flags, weight);
        objects.push_back(newObject);
    }
    return objects;
}

vector<Resets> GameDataImporter::returnResets(GameModel& gameModel, const YAML::Node& RESETS) {
    vector<Resets> resets = {};
    for(const auto& RESET : RESETS) {
        string resetAction = " ";
        if(RESET["action"]) {
            resetAction = RESET["action"].as<string>();
        }

        string itemID = " ";
        if(RESET["id"]) {
            itemID = RESET["id"].as<string>();
        }

        string roomID = " ";
        if(RESET["room"]) {
            roomID = RESET["room"].as<string>();
        }

        string stateOfDoor = " ";
        if(RESET["state"]) {
            stateOfDoor = RESET["state"].as<string>();
        }

        int limit = 1;
        if(RESET["limit"]) {
            limit = RESET["limit"].as<int>();
        }

        int slot = 0;
        if(RESET["slot"]) {
            slot = RESET["slot"].as<int>();
        }
        Resets newReset = Resets(resetAction, itemID, roomID, stateOfDoor, slot, limit);
        resets.push_back(newReset);
    }
    return resets;
}

void GameDataImporter::loadSpells(const YAML::Node& DEFENSE_SPELLS) {
    for (const auto& DEFENSE : DEFENSE_SPELLS) {
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
void GameDataImporter::loadShops(GameModel& gameModel, const YAML::Node& SHOPS){

    for(const auto& SHOP : SHOPS){
        //no data on shops in mgoose file

    }

}
