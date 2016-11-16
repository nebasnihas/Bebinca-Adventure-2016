#ifndef ADVENTURE2016_YAMLFILEBUILDER_HPP
#define ADVENTURE2016_YAMLFILEBUILDER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <assert.h>
#include <fstream>
#include "../../lib/yaml-cpp/src/nodebuilder.h"
#include "yaml-cpp/yaml.h"

class YamlAreaBuilder
{
public:

    static YAML::Node populateAreasNode(std::vector<std::string> desc, std::vector<std::string> extended_descriptions,
                                        std::string id, std::string name, std::vector<std::string> doorDesc,
                                        std::vector<std::string> keywords, std::string doorDir, std::string doorTo);

private:

};


#endif //ADVENTURE2016_YAMLFILEBUILDER_HPP
