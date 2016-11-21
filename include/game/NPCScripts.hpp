#ifndef ADVENTURE2016_NPCSCRIPTS_HPP
#define ADVENTURE2016_NPCSCRIPTS_HPP

//#include <game/Character.hpp>
#include <vector>
#include <string>
#include <unordered_map>

class NPCScripts {
public:

    NPCScripts(     const std::unordered_map<std::string, std::vector<std::string>>& qualifierCommandsMap,
                    const std::string& scriptingName,
                    const std::string& scriptingDescription
    );

    std::unordered_map<std::string, std::vector<std::string>> getQualifierCommandsMap() const;
    std::string getScriptingName() const;
    std::string getScriptingDescription() const;

    //Defaults
    static const std::vector<std::string> defaultCommand;
    static const std::vector<std::string> defaultQualifier;
    static const std::string defaultScriptingName;
    static const std::vector<std::string> defaultScriptingDescription;

private:
    std::unordered_map<std::string, std::vector<std::string>> qualifierCommandsMap;
    std::string scriptingName;
    std::string scriptingDescription;
};

#endif //ADVENTURE2016_NPCSCRIPTS_HPP
