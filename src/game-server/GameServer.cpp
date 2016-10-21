/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <boost/lexical_cast.hpp>
#include "glog/logging.h"
#include "ServerLoop.hpp"
#include "yaml-cpp/yaml.h"

const std::string PORT_KEY = "port";
const std::string TICKS_KEY = "ticks-per-second";
const std::string MAPFILE_KEY = "map-file";
const std::string COMMANDS_KEY = "commands";

std::string configFilePath = "config.yml";

void initGoogleLog(const YAML::Node& configRootNode) {
    if (auto googleLogSettings = configRootNode["google-log"]) {
        if (auto logDirectory = googleLogSettings["log-directory"]) {
            FLAGS_log_dir = logDirectory.as<std::string>();
        }
    }

    google::InitGoogleLogging("GameServer");
}

template<typename T>
T getConfigValueAs(const std::string key, const YAML::Node& configRootNode) {
    if (!configRootNode[key]) {
        std::cerr << "Error. Configuration file: '" << configFilePath << "' does not contain the required key: " << key
                  << std::endl;
        exit(-1);
    }

    return configRootNode[key].as<T>();
}

int main(int argc, char *argv[]) {
    const std::string usage = "Searching for config.yml in current path. "
            "Pass the path of the config file as a command line argument if you want to use that instead.";

    if (argc < 2) {
        std::cout << usage << std::endl;
    } else {
        configFilePath = argv[1];
    }

    //configuration
    YAML::Node root = YAML::LoadFile(configFilePath);
    auto port = getConfigValueAs<unsigned short>(PORT_KEY, root);
    auto mapFile = getConfigValueAs<std::string>(MAPFILE_KEY, root);
    auto ticksPerSecond = getConfigValueAs<unsigned int>(TICKS_KEY, root);
    auto commandBindings = root[COMMANDS_KEY];

    initGoogleLog(root);

    //start server
    ServerLoop loop{port, mapFile, commandBindings};
    Looper looper{ticksPerSecond};
    looper.run(loop);

    return 0;
}

