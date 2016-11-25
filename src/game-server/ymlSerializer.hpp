//
//
//

#ifndef ADVENTURE2016_YMLSERIALIZER_HPP
#define ADVENTURE2016_YMLSERIALIZER_HPP

#include <iostream>
#include "yaml-cpp/yaml.h"
#include "../../include/game/Character.hpp"
#include <string>


class ymlSerializer {

public:
    static Character load_from_file(const std::string& username);
    static void save_to_file (const Character& c, const std::string& username);


};


#endif //ADVENTURE2016_YMLSERIALIZER_HPP
