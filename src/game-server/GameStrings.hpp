#ifndef ADVENTURE2016_GAMESTRINGS_H
#define ADVENTURE2016_GAMESTRINGS_H


#include <string>
#include "StringResourceImporter.hpp"

namespace GameStringKeys {
    const std::string INVALID_DIR = "invalid-direction";
    const std::string INVALID_TGT = "invalid-target";

    const std::string PLAYERS_AREA = "players-list-area";
    const std::string PLAYERS_WORLD = "players-list-world";
    const std::string EXITS_AREA = "exits-list";

    const std::string UNSPECIFIED_PLAYER = "unspecified-player";
    const std::string UNSPECIFIED_TARGET = "unspecified-target";
    const std::string UNSPECIFIED_EXIT = "unspecified-exit";

    const std::string PLAYER = "player";
    const std::string OBJECT = "object";

    const std::string GLOBAL_CHANNEL = "global-tag";
    const std::string AREA_CHANNEL = "area-tag";
    const std::string PRIVATE_CHANNEL = "private-tag";
}

class GameStrings {
public:
    static const std::string get(const std::string& key) {
        return StringResourceImporter::getInstance().getString(key);
    }

};


#endif //ADVENTURE2016_GAMESTRINGS_H
