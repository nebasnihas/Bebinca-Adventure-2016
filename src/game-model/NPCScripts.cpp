#include <game/NPCScripts.hpp>

NPCScripts::NPCScripts(const std::pair<std::string, std::string> &qualifierCommandsPair,
                       const std::string &scriptingName,
                       const std::string &scriptingDescription
                      )

                      :
                       qualifierCommandsPair(qualifierCommandsPair)
                      , scriptingName(scriptingName)
                      , scriptingDescription(scriptingDescription)

                      {

                      }

const std::vector<std::string> NPCScripts::defaultCommand = {" ", " "};
const std::vector<std::string> NPCScripts::defaultQualifier = {" ", " "};
const std::string NPCScripts::defaultScriptingName = {" ", " "};
const std::vector<std::string> NPCScripts::defaultScriptingDescription = {" ", " "};

std::pair<std::string, std::string> NPCScripts::getQualifierCommandsPair() const {
    return qualifierCommandsPair;
};

std::string NPCScripts::getScriptingName() const {
    return scriptingName;
};

std::string NPCScripts::getScriptingDescription() const {
    return scriptingDescription;
}