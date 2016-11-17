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

    static YAML::Node populateAreasNode(const std::vector<std::string>& desc, const std::vector<std::string>& extended_descriptions,
                                        const std::string& id, const std::string& name, const std::vector<std::string>& doorDesc,
                                        const std::vector<std::string>& keywords, const std::string& doorDir, const std::string& doorTo);

private:

};


#endif //ADVENTURE2016_YAMLFILEBUILDER_HPP
