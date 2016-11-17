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