//
//
//

#ifndef ADVENTURE2016_YMLSERIALIZER_HPP
#define ADVENTURE2016_YMLSERIALIZER_HPP

#include <iostream>
#include "yaml-cpp/yaml.h"
#include "../../include/game/Character.hpp"
#include <string>
#include <glog/logging.h>
#include <fstream>

class ymlSerializer {

private:
    static std::string get_saveloc(const std::string& username);

public:
    static Character load_from_file(const std::string& username);
    static void save_to_file (const Character& c, const std::string& username);


};


#endif //ADVENTURE2016_YMLSERIALIZER_HPP
