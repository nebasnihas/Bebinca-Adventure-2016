#include <glog/logging.h>
#include <boost/algorithm/string.hpp>
#include "EditCommand.hpp"
#include "EditMessageBuilder.hpp"
#include "game/GameModel.hpp"
#include "game/GameDataImporter.hpp"

namespace {
enum class ArgToken {
    AREA,
    SUBMIT,
};

const std::string AREA_TOKEN = "area";
const std::string COMMIT_TOKEN = "submit";

std::unordered_map<std::string, ArgToken> tokenMap{
        {AREA_TOKEN,   ArgToken::AREA},
        {COMMIT_TOKEN, ArgToken::SUBMIT},
};

std::unique_ptr<MessageBuilder> areaInUse(const networking::Connection& player) {
    return DisplayMessageBuilder{"Area is currently being edited"}.addClient(player);
}

std::unique_ptr<MessageBuilder> alreadyEditing(const networking::Connection& player) {
    protocols::EditResponse editResponse;
    editResponse.success = false;
    editResponse.editType = protocols::EditType::AREA;
    return std::make_unique<EditMessageBuilder>(player, editResponse);
}

std::unique_ptr<MessageBuilder> notEditing(const networking::Connection& player) {
    return DisplayMessageBuilder{"You are not current editing an area"}.addClient(player);
}

}

EditCommand::EditCommand(GameModel& gameModel) : gameModel{gameModel} {}

std::unique_ptr<MessageBuilder> EditCommand::execute(const gsl::span<std::string, -1> arguments,
                                                     const PlayerInfo& player) {
    if (arguments.length() == 0) {
        return DisplayMessageBuilder{"Use the 'area [id]' argument to begin"}
                .addClient(player.clientID)
                .setSender(GameStrings::get(GameStringKeys::SERVER_NAME));
    }

    auto argument = tokenMap.find(arguments[0]);
    if (argument == tokenMap.end()) {
        return DisplayMessageBuilder{"Invalid argument to the edit command"}
                .addClient(player.clientID)
                .setSender(GameStrings::get(GameStringKeys::SERVER_NAME));
    }

    switch (argument->second) {
        case ArgToken::AREA:
            return editArea(arguments, player);
        case ArgToken::SUBMIT:
            return saveChanges(arguments, player);
    }
}

std::unique_ptr<MessageBuilder> EditCommand::editArea(const gsl::span<std::string, -1> arguments,
                                                      const PlayerInfo& player) {
    //arguments: area [areaid]
    CHECK(!arguments.empty()) << "Arguments should not be empty";

    Area* area;
    //Didnt specify area id
    if (arguments.size() == 1) {
        auto character = gameModel.getCharacterByID(player.playerID);
        area = gameModel.getAreaByID(character->getAreaID());
    } else {
        area = gameModel.getAreaByID(arguments[1]);
        if (area == nullptr) {
            auto newArea = Area{arguments[1]};
            area = &newArea;
        }
    }

    return addAreaToEdit(*area, player);
}

std::unique_ptr<MessageBuilder> EditCommand::addAreaToEdit(const Area& area, const PlayerInfo& player) {
    protocols::EditResponse editResponse;
    editResponse.success = true;
    editResponse.editType = protocols::EditType::AREA;

    if (playerIsEditing(player.playerID)) {
        editResponse.success = false;
        editResponse.data = YAML::Node{areaEditMap.at(player.playerID)};
        return std::make_unique<EditMessageBuilder>(player.clientID, editResponse);
    }

    if (areas.count(area.getID()) == 1) {
        return areaInUse(player.clientID);
    }

    areas.emplace(area.getID());
    areaEditMap.emplace(player.playerID, area);

    editResponse.data = YAML::Node{area};
    return std::make_unique<EditMessageBuilder>(player.clientID, editResponse);
}

bool EditCommand::playerIsEditing(const std::string& player) {
    return areaEditMap.count(player) == 1;
}

std::unique_ptr<MessageBuilder> EditCommand::saveChanges(const gsl::span<std::string, -1> arguments,
                                                         const PlayerInfo& player) {
    if (!playerIsEditing(player.playerID)) {
        return notEditing(player.clientID);
    }

    auto asYamlStr = boost::algorithm::join(arguments.subspan(1, arguments.length() - 1), "");
    auto areaYaml = YAML::Load(asYamlStr);

    auto area = areaEditMap.at(player.playerID);
    gameModel.addArea(area);

    areaEditMap.erase(player.playerID);
    areas.erase(area.getID());

    return DisplayMessageBuilder{"Saved changes to " + area.getID()}.addClient(player.clientID);
}