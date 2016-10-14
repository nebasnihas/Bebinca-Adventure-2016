

#include "GameFunctions.hpp"

using namespace std;

GameFunctions::GameFunctions(Controller &controller) : controller{controller}, gameModel{controller.getGameModel()}, allClients{controller.getAllClients()} {
    controller.registerCommand(Command{"look", std::bind(&GameFunctions::look, this, _1, _2)});
    controller.registerCommand(Command{"go", std::bind(&GameFunctions::move, this, _1, _2)});
    controller.registerCommand(Command{"players", std::bind(&GameFunctions::listPlayers, this, _1, _2)});
    controller.registerCommand(Command{"exits", std::bind(&GameFunctions::listExits, this, _1, _2)});
    controller.registerCommand(Command{"say", std::bind(&GameFunctions::say, this, _1, _2)});
}

DisplayMessageBuilder GameFunctions::look(const std::vector<std::string> &targets, const PlayerInfo &player) {
    auto areaID = gameModel.getCharacterByID(player.playerID)->getAreaID();
    string description;
    if (targets.empty()) {
        description = gameModel.getAreaDescription(areaID);
    }
    else {
        auto targetEntity = targets[0];
        description = gameModel.getEntityDescription(areaID, targetEntity);
    }
    return DisplayMessageBuilder::createMessage(description).addClient(player.clientID).setSender(DisplayMessageBuilder::SENDER_SERVER);
}

DisplayMessageBuilder GameFunctions::move(const std::vector<std::string> &targets, const PlayerInfo &player) {
    string message;
    if (gameModel.moveCharacter(player.playerID, targets[0])) {
        auto areaID = gameModel.getCharacterByID(player.playerID)->getAreaID();
        message = gameModel.getAreaDescription(areaID);
    }
    else {
        message = "Area not found";
    }
    return DisplayMessageBuilder::createMessage(message).addClient(player.clientID).setSender(DisplayMessageBuilder::SENDER_SERVER);
}

DisplayMessageBuilder GameFunctions::listPlayers(const std::vector<std::string> &targets, const PlayerInfo &player) {
    auto areaID = getPlayerAreaID(player);
    string message = "Players in the area:\n";
    for (auto player: gameModel.getCharacterIDsInArea(areaID)) {
        message += gameModel.getCharacterByID(player)->getName() + " \n";
    }
    return DisplayMessageBuilder::createMessage(message).addClient(player.clientID).setSender(DisplayMessageBuilder::SENDER_SERVER);
}

DisplayMessageBuilder GameFunctions::listExits(const std::vector<std::string> &targets, const PlayerInfo &player) {
    auto areaID = getPlayerAreaID(player);
    auto connectedAreaMap = *gameModel.getConnectedAreas(areaID);
    string message;
    for (auto mapEntry: connectedAreaMap) {
        message += mapEntry.first + "\n";
    }
    return DisplayMessageBuilder::createMessage(message).addClient(player.clientID).setSender(DisplayMessageBuilder::SENDER_SERVER);
}

DisplayMessageBuilder GameFunctions::say(const std::vector<std::string> &targets, const PlayerInfo &player) {
    std::string message;
    for (const auto& target : targets) {
        message += target + " ";
    }
    return DisplayMessageBuilder::createMessage(message).addClients(controller.getAllClients()).setSender(player.playerID);
}

string GameFunctions::getPlayerAreaID(const PlayerInfo &player) {
    return gameModel.getCharacterByID(player.playerID)->getAreaID();
}



