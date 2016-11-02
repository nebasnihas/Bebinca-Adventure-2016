#include <iostream>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/yaml.h>
#include "StringResourceImporter.hpp"



std::string StringResourceImporter::getString(const std::string& key) {
//    std::cout << stringNode["lookCommand"].as<std::string>();
//    return stringNode[key].as<std::string>();
    if (!stringNode[key]) {
        std::cerr << "ERROR: Corresponding String not Found for " << key << std::endl;
        exit(-1);
    }
    return stringNode[key].as<std::string>();
}


StringResourceImporter::StringResourceImporter(const std::string &stringResourcePath) {
    const auto stringFile = YAML::LoadFile(stringResourcePath);
    StringResourceImporter::stringNode = stringFile["strings"];
    if (!stringNode) {
        std::cerr << "ERROR: String file not found" << std::endl;
        exit(-1);
    }
}


