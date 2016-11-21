#include <game/NPCScripts.hpp>

NPCScripts::NPCScripts(const std::unordered_map<std::string, std::vector<std::string>> &qualifierCommandsMap,
                       const std::string &scriptingName,
                       const std::string &scriptingDescription
                      )

                      :
                       qualifierCommandsMap(qualifierCommandsMap)
                      , scriptingName(scriptingName)
                      , scriptingDescription(scriptingDescription)

                      {

                      }

const std::vector<std::string> NPCScripts::defaultCommand = {" ", " "};
const std::vector<std::string> NPCScripts::defaultQualifier = {" ", " "};
const std::string NPCScripts::defaultScriptingName = {" ", " "};
const std::vector<std::string> NPCScripts::defaultScriptingDescription = {" ", " "};

std::unordered_map<std::string, std::vector<std::string>> NPCScripts::getQualifierCommandsMap() const {
    return qualifierCommandsMap;
};

std::string NPCScripts::getScriptingName() const {
    return scriptingName;
};

std::string NPCScripts::getScriptingDescription() const {
    return scriptingDescription;
}