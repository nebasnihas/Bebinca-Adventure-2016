#ifndef ADVENTURE2016_NPCSCRIPTS_HPP
#define ADVENTURE2016_NPCSCRIPTS_HPP

//#include <game/Character.hpp>
#include <vector>
#include <string>

class NPCScripts {
public:

    NPCScripts(     const std::pair<std::string, std::string>& qualifierCommandsPair,
                    const std::string& scriptingName,
                    const std::string& scriptingDescription
    );

    std::pair<std::string, std::string> getQualifierCommandsPair() const;
    std::string getScriptingName() const;
    std::string getScriptingDescription() const;

    //Defaults
    static const std::vector<std::string> defaultCommand;
    static const std::vector<std::string> defaultQualifier;
    static const std::string defaultScriptingName;
    static const std::vector<std::string> defaultScriptingDescription;

private:
    std::pair<std::string, std::string> qualifierCommandsPair;
    std::string scriptingName;
    std::string scriptingDescription;
};

#endif //ADVENTURE2016_NPCSCRIPTS_HPP
