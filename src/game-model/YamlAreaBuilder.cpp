#include <game/YamlAreaBuilder.hpp>


//void YamlAreaBuilder::createFile(const std::string &fileName) {
//
//}

YAML::Node YamlAreaBuilder::populateAreasNode(std::vector<std::string> desc, std::vector<std::string> extended_descriptions,
                                              std::string id, std::string name, std::vector<std::string> doorDesc,
                                              std::vector<std::string> keywords, std::string doorDir, std::string doorTo)
{
    //top level rooms node
    YAML::Node ROOMS;

    ROOMS["desc"].push_back(desc);
    ROOMS["extended_descriptions"].push_back(extended_descriptions);
    ROOMS["id"].push_back(id);
    ROOMS["name"].push_back(name);

    ROOMS["doors"]["dir"].push_back(doorDir);
    ROOMS["doors"]["to"].push_back(doorTo);
    ROOMS["doors"]["desc"].push_back(doorDesc);
    ROOMS["doors"]["keywords"].push_back(keywords);

    return ROOMS;
}

//////////////////////Try #2

//    ROOMS*["desc"] = desc;
//    ROOMS["extended_descriptions"] = extended_descriptions;
//    ROOMS["id"] = id;
//    ROOMS["name"] = name;
//
//    ROOMS["doors"]["dir"] = doorDir;
//    ROOMS["doors"]["to"] = doorTo;
//    ROOMS["doors"]["desc"] = doorDesc;
//    ROOMS["doors"]["keywords"] = keywords;

///////////////////////Try #1

//
//    YAML::Node descNode;
//    YAML::Node extendedDescriptionsNode;
//    YAML::Node idNode;
//    YAML::Node nameNode;
//
//    ROOMS["ROOMS"] = descNode;
//    ROOMS["ROOMS"] = extendedDescriptionsNode;
//    ROOMS["ROOMS"] = idNode;
//    ROOMS["ROOMS"] = nameNode;
//
//    descNode["desc"].push_back(desc);
//    extendedDescriptionsNode["extended_descriptions"].push_back(extended_descriptions);
//    idNode["desc"].push_back(id);
//    nameNode["name"].push_back(name);
//
//    YAML::Node doorNode;
//
//    YAML::Node doorsDescNode;
//    YAML::Node doorsDirNode;
//    YAML::Node doorsKeywordsNode;
//    YAML::Node doorsToNode;
//
//    doorNode["doors"] = doorsDescNode;
//    doorNode["doors"] = doorsDirNode;
//    doorNode["doors"] = doorsKeywordsNode;
//    doorNode["doors"] = doorsToNode;
//
//    doorsDescNode["desc"].push_back(doorDesc);
//    doorsDirNode["dir"].push_back(doorDir);
//    doorsKeywordsNode["keywords"].push_back(keywords);
//    doorsToNode["to"].push_back(doorTo);