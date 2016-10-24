#include "GameFunctions.hpp"


GameFunctions::GameFunctions(Controller &controller) : controller{controller}, gameModel{controller.getGameModel()}, allClients{controller.getAllClients()} {
    controller.registerCommand(Command{"look", std::bind(&GameFunctions::look, this, std::placeholders::_1, std::placeholders::_2)});
    controller.registerCommand(Command{"go", std::bind(&GameFunctions::move, this, std::placeholders::_1, std::placeholders::_2)});
    controller.registerCommand(Command{"players", std::bind(&GameFunctions::listPlayers, this, std::placeholders::_1, std::placeholders::_2)});
    controller.registerCommand(Command{"exits", std::bind(&GameFunctions::listExits, this, std::placeholders::_1, std::placeholders::_2)});
    controller.registerCommand(Command{"say", std::bind(&GameFunctions::say, this, std::placeholders::_1, std::placeholders::_2)});
}

std::unique_ptr<MessageBuilder> GameFunctions::look(const std::vector<std::string>& targets, const PlayerInfo& player) {
    auto areaID = gameModel.getCharacterByID(player.playerID)->getAreaID();
    std::string description;

    if (targets.empty()) {
        description = gameModel.getAreaDescription(areaID);
    } else {
        auto targetObject = targets[0];
        description = gameModel.getObjectDescription(areaID, targetObject);
    }

    return DisplayMessageBuilder::createMessage(description).addClient(player.clientID).setSender(DisplayMessageBuilder::SENDER_SERVER);
}

std::unique_ptr<MessageBuilder> GameFunctions::move(const std::vector<std::string>& targets, const PlayerInfo& player) {
    std::string message;
    if (gameModel.moveCharacter(player.playerID, targets[0])) {
        auto areaID = gameModel.getCharacterByID(player.playerID)->getAreaID();
        message = gameModel.getAreaDescription(areaID);
    } else {
        message = "Area not found";
    }

    return DisplayMessageBuilder::createMessage(message).addClient(player.clientID).setSender(DisplayMessageBuilder::SENDER_SERVER);
}

std::unique_ptr<MessageBuilder> GameFunctions::listPlayers(const std::vector<std::string>& targets,
                                                           const PlayerInfo& player) {
    auto areaID = getPlayerAreaID(player);
    std::string message = "Players in the area:\n";
    for (const auto& playerIDs: gameModel.getCharacterIDsInArea(areaID)) {
        message += gameModel.getCharacterByID(playerIDs)->getName() + " \n";
    }

    return DisplayMessageBuilder::createMessage(message).addClient(player.clientID).setSender(DisplayMessageBuilder::SENDER_SERVER);
}

std::unique_ptr<MessageBuilder> GameFunctions::listExits(const std::vector<std::string>& targets,
                                                         const PlayerInfo& player) {
    auto areaID = getPlayerAreaID(player);
    auto connectedAreaMap = *gameModel.getConnectedAreas(areaID);
    std::string message = "Exits:\n";
    for (auto mapEntry: connectedAreaMap) {
        message += mapEntry.first + "\n";
    }

    return DisplayMessageBuilder::createMessage(message).addClient(player.clientID).setSender(DisplayMessageBuilder::SENDER_SERVER);
}

std::unique_ptr<MessageBuilder> GameFunctions::say(const std::vector<std::string>& targets, const PlayerInfo& player) {
    std::string message;
    for (const auto& target : targets) {
        message += target + " ";
    }

    return DisplayMessageBuilder::createMessage(message).addClients(controller.getAllClients()).setSender(player.playerID);
}

std::string GameFunctions::getPlayerAreaID(const PlayerInfo &player) {
    return gameModel.getCharacterByID(player.playerID)->getAreaID();
}



