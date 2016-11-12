#ifndef ADVENTURE2016_YAMLFILEBUILDER_HPP
#define ADVENTURE2016_YAMLFILEBUILDER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <assert.h>
#include <fstream>
#include "../../lib/yaml-cpp/src/nodebuilder.h"

class YamlAreaBuilder
{
public:

    //static void createFile(const std::string& fileName);

    static YAML::Node* populateAreasNode(std::vector<std::string> desc, std::vector<std::string> extended_descriptions, std::string id, std::string name, std::vector<std::string> doorDesc, std::vector<std::string> keywords, std::string doorDir, std::string doorTo);

    //static std::string getFilePath(const std::string& fileName);

private:

};


#endif //ADVENTURE2016_YAMLFILEBUILDER_HPP
