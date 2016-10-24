#include <iostream>
#include "GameFunctions.hpp"


GameFunctions::GameFunctions(Controller &controller) : controller{controller}, gameModel{controller.getGameModel()}, allClients{controller.getAllClients()} {
    controller.registerCommand(Command{"look", std::bind(&GameFunctions::look, this, std::placeholders::_1, std::placeholders::_2)});
    controller.registerCommand(Command{"go", std::bind(&GameFunctions::move, this, std::placeholders::_1, std::placeholders::_2)});
    controller.registerCommand(Command{"players", std::bind(&GameFunctions::listPlayers, this, std::placeholders::_1, std::placeholders::_2)});
    controller.registerCommand(Command{"exits", std::bind(&GameFunctions::listExits, this, std::placeholders::_1, std::placeholders::_2)});
    controller.registerCommand(Command{"say", std::bind(&GameFunctions::say, this, std::placeholders::_1, std::placeholders::_2)});
    controller.registerCommand(Command{"shout", std::bind(&GameFunctions::shout, this, std::placeholders::_1, std::placeholders::_2)});
    controller.registerCommand(Command{"whisper", std::bind(&GameFunctions::whisper, this, std::placeholders::_1, std::placeholders::_2)});
}

std::unique_ptr<MessageBuilder> GameFunctions::look(const std::vector<std::string>& targets, const PlayerInfo& player) {
    auto areaID = getPlayerAreaID(player);
    std::string description;

    if (targets.empty()) {
        description = gameModel.getAreaDescription(areaID);
    } else {
        auto targetEntity = targets[0];
        description = gameModel.getEntityDescription(areaID, targetEntity);
    }

    return DisplayMessageBuilder::createMessage(description)
            .addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}

std::unique_ptr<MessageBuilder> GameFunctions::move(const std::vector<std::string>& targets, const PlayerInfo& player) {
    std::string message;
    if (gameModel.moveCharacter(player.playerID, targets[0])) {
        auto areaID = gameModel.getCharacterByID(player.playerID)->getAreaID();
        message = gameModel.getAreaDescription(areaID);
    } else {
        //TODO: extract into string resource
        message = "Area not found";
    }

    return DisplayMessageBuilder::createMessage(message)
            .addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}

std::unique_ptr<MessageBuilder> GameFunctions::listPlayers(const std::vector<std::string>& targets, const PlayerInfo& player) {
    auto areaID = getPlayerAreaID(player);
    //TODO: extract into string resource file
    std::string message = "Players in the area:\n";
    for (const auto& playerID: gameModel.getCharacterIDsInArea(areaID)) {
        message += gameModel.getCharacterByID(playerID)->getName() + " \n";
    }

    return DisplayMessageBuilder::createMessage(message)
            .addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}

std::unique_ptr<MessageBuilder> GameFunctions::listExits(const std::vector<std::string>& targets, const PlayerInfo& player) {
    auto areaID = getPlayerAreaID(player);
    auto connectedAreaMap = *gameModel.getConnectedAreas(areaID);
    //TODO: extract into string resource file
    std::string message = "Exits:\n";
    for (auto mapEntry: connectedAreaMap) {
        message += mapEntry.first + "\n";
    }

    return DisplayMessageBuilder::createMessage(message)
            .addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}

std::unique_ptr<MessageBuilder> GameFunctions::say(const std::vector<std::string>& targets, const PlayerInfo& player) {
    std::string message;
    for (const auto& target : targets) {
        message += target + " ";
    }

    return DisplayMessageBuilder::createMessage(message)
            .addClients(controller.getAllClients())
            .setSender(player.playerID);
}

std::unique_ptr<MessageBuilder> GameFunctions::shout(const std::vector<std::string> &targets, const PlayerInfo &player) {
    std::string message;
    for (const auto& target : targets) {
        message += target + " ";
    }
    auto areaID = getPlayerAreaID(player);

    std::vector<networking::Connection> localClients;
    for (const auto &player: gameModel.getCharacterIDsInArea(areaID)) {
        localClients.push_back(controller.getClientID(player).get());
    }

    return DisplayMessageBuilder::createMessage(message)
            .addClients(localClients)
            .setSender(player.playerID);
}

std::unique_ptr<MessageBuilder> GameFunctions::whisper(const std::vector<std::string> &targets, const PlayerInfo &player) {
    auto targetPlayerID = *targets.begin();
    std::string message;
    auto targetClient = controller.getClientID(targetPlayerID);

    if (!targetClient) {
        //TODO: extract into string resources
        message = "Player Not Found";
        return DisplayMessageBuilder::createMessage(message)
                .addClient(player.clientID)
                .setSender(DisplayMessageBuilder::SENDER_SERVER);
    }
    else {
        auto targetsIterator = targets.begin();
        std::advance(targetsIterator, 1);
        for (targetsIterator; targetsIterator != targets.end(); targetsIterator++ ) {
            message += *targetsIterator + " ";
        }
        return DisplayMessageBuilder::createMessage(message)
                .addClient(targetClient.get())
                .setSender(player.playerID);
    }
}




std::string GameFunctions::getPlayerAreaID(const PlayerInfo &player) {
    return gameModel.getCharacterByID(player.playerID)->getAreaID();
}



