#ifndef ADVENTURE2016_STRINGRESOURCEIMPORTER_H
#define ADVENTURE2016_STRINGRESOURCEIMPORTER_H

#include <string>

enum class GameLang {
    ENGLISH,
};


class StringResourceImporter {

public:
    static StringResourceImporter& getInstance() {
        static StringResourceImporter instance{"data/strings.yml"};
        return instance;
    }
    std::string getString(const std::string& key);



private:
    YAML::Node stringNode;
    StringResourceImporter(const std::string& stringResourcePath);
};


#endif //ADVENTURE2016_STRINGRESOURCEIMPORTER_H
