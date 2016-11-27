#include "game/GameDataImporter.hpp"
#include "ExpressionExtractor.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::unordered_map;

YAML::Node GameDataImporter::getRootYAMLNode(const std::string& fileName) {
    return YAML::LoadFile(fileName);
}

std::unordered_map<std::string, NPC> GameDataImporter::returnNPCS(const YAML::Node& NPCS){
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
        string sLongDescription = boost::algorithm::join(longdesc, " ");

        Inventory inventory;
        std::string areaID;

        unordered_map<string, NPCScripts> NPCScriptings;
        //Scripting Attributes
        if(nodeNPC["programs"]) {

            const YAML::Node programs = nodeNPC["programs"];
            for (const auto &program : programs) {
                vector<string> scriptingCommands = program["commands"] ? program["commands"].as<vector<string>>() : NPCScripts::defaultCommand;
				for (auto& command: scriptingCommands) {
					boost::trim(command);
				}
                string qualifier = program["qualifier"] ? program["qualifier"].as<string>() : NPCScripts::defaultQualifier;
                string scriptingName = program["name"] ? program["name"].as<string>() : NPCScripts::defaultScriptingName;
                vector<string> scriptingDescription = program["description"] ? program["description"].as<vector<string>>() : NPCScripts::defaultScriptingDescription;

                boost::replace_all(qualifier, "~", "");
                string scriptingDescriptionString = boost::algorithm::join(scriptingDescription, " ");

                std::pair<string, vector<string>> qualifierCommandsPair (qualifier, scriptingCommands);

                NPCScripts newNPCScript = NPCScripts(qualifier, scriptingCommands, scriptingName, scriptingDescriptionString);

                NPCScriptings.insert(std::pair<string, NPCScripts>(qualifier,newNPCScript));
            }
        }

        NPC newNPC = NPC(npcID, shortdesc, hit, damage, level, exp, armor, gold, areaID, thac0,
                              sDescription, keywords, sLongDescription, NPCScriptings);
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
        //vector<string> extended_descriptions = ROOM["extended_descriptions"].as<vector<string>>();
        unordered_map<string, string> exKeywordToDesc;
		for (const auto& EX_DESC : ROOM["extended_descriptions"]) {
			auto ex_desc_list = EX_DESC["desc"].as<vector<string>>();
			auto ex_desc = boost::join(ex_desc_list, " ");
			string ex_keyword = EX_DESC["keywords"].as<vector<string>>()[0];
			exKeywordToDesc[ex_keyword] = ex_desc;
		}

		vector<string> extended_descriptions;
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
        Area newArea = Area(id, name, doorsMap, description, exKeywordToDesc);
        rooms.push_back(newArea);
    }
    return rooms;
}

std::unordered_map<std::string, Object> GameDataImporter::returnObjects(const YAML::Node& OBJECTS){
    unordered_map<std::string, Object> objects;

    /*
    Split up objects in YML file and store them in the objects class
    */

    for(const auto& OBJECT : OBJECTS){
        int cost = OBJECT["cost"] ? OBJECT["cost"].as<int>() : Object::defaultCost;
        int weight = OBJECT["weight"] ? OBJECT["weight"].as<int>() : Object::defaultWeight;

        string objectID = OBJECT["id"] ? OBJECT["id"].as<string>() : Object::defaultObjectID;
        string item_type = OBJECT["item_type"] ? OBJECT["item_type"].as<string>() : Object::defaultItemType;
        string shortdesc = OBJECT["shortdesc"] ? OBJECT["shortdesc"].as<string>() : Object::defaultShortDesc;

        vector<string> attributes = OBJECT["attributes"] ? OBJECT["attributes"].as<vector<string>>() : Object::defaultAttributes;
        vector<string> keywords = OBJECT["keywords"] ? OBJECT["keywords"].as<vector<string>>() : Object::defaultKeywords;
        vector<string> longdesc = OBJECT["longdesc"] ? OBJECT["longdesc"].as<vector<string>>() : Object::defaultLongDesc;
        vector<string> wear_flags = OBJECT["wear_flags"] ? OBJECT["wear_flags"].as<vector<string>>() : Object::defaultWearFlags;

        string description = boost::algorithm::join(longdesc, " ");

        Object newObject = Object(attributes, cost, objectID, item_type, keywords, description, shortdesc, wear_flags, weight);
        objects.insert(std::pair<std::string, Object> (shortdesc, newObject));
    }
    return objects;
}

vector<Resets> GameDataImporter::returnResets(const YAML::Node& RESETS) {
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

vector<Spell> GameDataImporter::getSpells(const YAML::Node& SPELLS) {

    std::vector<Spell> returnSpells;

    auto& DEFENSE_SPELLS = SPELLS["defense"];
    for (const auto& DEFENSE : DEFENSE_SPELLS) {
        Spell spell;
        if (tryParseSpell(DEFENSE, SpellType::DEFENSE, spell)) {
            returnSpells.push_back(spell);
        }
    }

    auto& OFFENSE_SPELLS = SPELLS["offense"];
    for (const auto& OFFENSE : OFFENSE_SPELLS) {
        Spell spell;
        if (tryParseSpell(OFFENSE, SpellType::OFFENSE, spell)) {
            returnSpells.push_back(spell);
        }
    }

    return returnSpells;

}

bool GameDataImporter::tryParseSpell(const YAML::Node &SPELL, SpellType spellType, Spell &retSpell) {

    // Required values, exit if not present
    if (!SPELL["Mana"] || !SPELL["Name"] || !SPELL["Minlevel"]) {
        return false;
    }

    int mana = SPELL["Mana"].as<int>();
    string name = SPELL["Name"].as<string>();
    int minLevel = SPELL["Minlevel"].as<int>();

    // Exit if effect cannot be parsed
    string effect;
    if (SPELL["Effect"]) {
        effect = SPELL["Effect"].as<string>();
    }
    bool successfulParse = tryExtractEffectsExpression(effect, effect);
    if (!successfulParse) {
        return false;
    }

    // Optional values
    int duration = 0;
    string wearoff;
    string hitchar;
    string hitroom;
    string hitvict;
    string dammsg;

    if (SPELL["Wearoff"]) {
        wearoff = SPELL["Wearoff"].as<string>();
    }

    if (SPELL["Dammsg"]) {
        dammsg = SPELL["Dammsg"].as<string>();
    }

    if (SPELL["Hitchar"]) {
        hitchar = SPELL["Hitchar"].as<string>();
    }

    if (SPELL["Hitroom"]) {
        hitroom = SPELL["Hitroom"].as<string>();
    }

    if (SPELL["Hitvict"]) {
        hitvict = SPELL["Hitvict"].as<string>();
    }

    if (SPELL["Duration"]) {
        duration = SPELL["Duration"].as<int>();
    }

    retSpell = Spell(name, mana, spellType, effect, hitchar, hitroom, hitvict);
    return true;
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
//void GameDataImporter::loadShops(GameModel& gameModel, const YAML::Node& SHOPS){
//
//    for(const auto& SHOP : SHOPS){
//        //no data on shops in mgoose file
//
//    }
//
//}
