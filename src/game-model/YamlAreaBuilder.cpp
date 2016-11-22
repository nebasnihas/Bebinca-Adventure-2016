#include <game/YamlAreaBuilder.hpp>


//void YamlAreaBuilder::createFile(const std::string &fileName) {
//
//}

YAML::Node YamlAreaBuilder::populateAreasNode(const std::vector<std::string>& desc, const std::vector<std::string>& extended_descriptions,
                                              const std::string& id, const std::string& name, const std::vector<std::string>& doorDir,
                                              const std::vector<std::string>& doorTo)
{
    //top level rooms node
    YAML::Node ROOMS;
    YAML::Node ROOM;
    YAML::Node doors;

      //ERROR:invalid node; this may result from using a map iterator as a sequence iterator, or vice-versa"
    ROOM["desc"].push_back(desc[0]);

    //may need a for loop for this all to populate
//
//    for (const auto &door : doorDir){
//        doors["dir"] = door;
//        doors["to"] = doorTo;
//
//    }

    for (int i = 0; i < doorDir.size(); i++) {
        YAML::Node temp;
        temp["dir"] = doorDir[i];
        temp["to"] = doorTo[i];
        ROOM["doors"].push_back(temp);
    }



    //ROOM["doors"].push_back(doors);
    ROOM["extended_descriptions"].push_back(extended_descriptions[0]);
    ROOM["id"] = id;
    ROOM["name"] = name;

    ROOMS.push_back(ROOM);

    return ROOMS;
}

//ERROR:invalid node; this may result from using a map iterator as a sequence iterator, or vice-versa"
//ROOM["desc"].push_back(desc);
//
//doors["door1"]["desc"].push_back(doorDesc);
//doors["door1"]["dir"].push_back(doorDir);
//doors["door1"]["keywords"].push_back(keywords);
//doors["door1"]["to"].push_back(doorTo);
//ROOM["doors"].push_back(doors);
//
//ROOM["extended_descriptions"].push_back(extended_descriptions);
//ROOM["id"].push_back(id);
//ROOM["name"].push_back(name);
//
//ROOMS["ROOM"].push_back(ROOM);

///////

///////

//ERROR:invalid node; this may result from using a map iterator as a sequence iterator, or vice-versa"
//ROOMS["desc"].push_back(desc);
//
//ROOMS["doors"]["desc"].push_back(doorDesc);
//ROOMS["doors"]["dir"].push_back(doorDir);
//ROOMS["doors"]["keywords"].push_back(keywords);
//ROOMS["doors"]["to"].push_back(doorTo);
//
//ROOMS["extended_descriptions"].push_back(extended_descriptions);
//ROOMS["id"].push_back(id);
//ROOMS["name"].push_back(name);

///////

///////

//ERROR: apphending to a non-sequence thrown in setUp() -- bad_pushback
//ROOM.push_back("desc");
//ROOM["desc"] = desc;
//ROOM.push_back("extended_descriptions");
//ROOM["extended_descriptions"] = extended_descriptions;
//ROOM.push_back("id");
//ROOM["id"] = id;
//ROOM.push_back("name");
//ROOM["name"] = name;
//
//ROOM.push_back("doors");
//ROOM["doors"]["dir"] = doorDir;
//ROOM.push_back("to");
//ROOM["doors"]["to"] = doorTo;
//ROOM.push_back("desc");
//ROOM["doors"]["desc"] = doorDesc;
//ROOM.push_back("doors");
//ROOM["doors"]["keywords"] = keywords;

///////

///////

//ERROR: apphending to a non-sequence thrown in setUp() --bad_pushback
//ROOM["desc"].push_back("desc");
//ROOM["desc"] = desc;
//ROOM["extended_descriptions"].push_back("extended_descriptions");
//ROOM["extended_descriptions"] = extended_descriptions;
//ROOM["id"].push_back("id");
//ROOM["id"] = id;
//ROOM["name"].push_back("name");
//ROOM["name"] = name;
//
//ROOM["doors"].push_back("doors");
//ROOM["doors"]["dir"] = doorDir;
//ROOM["to"].push_back("to");
//ROOM["doors"]["to"] = doorTo;
//ROOM["desc"].push_back("desc");
//ROOM["doors"]["desc"] = doorDesc;
//ROOM["doors"].push_back("doors");
//ROOM["doors"]["keywords"] = keywords;