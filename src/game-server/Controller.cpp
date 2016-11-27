#include <boost/algorithm/string.hpp>
#include <glog/logging.h>
#include "Controller.hpp"
#include "StringUtils.hpp"
#include "CommandList.hpp"
#include "PigLatinDecorator.hpp"
#include "MessageIO.hpp"
#include "ConnectionManager.hpp"
#include "game/GameModel.hpp"

using namespace networking;
using namespace std::placeholders;

Controller::Controller(GameModel& gameModel, MessageIO& messageIO, ConnectionManager& connectionManager,
                       const CommandConfig& commandCreator)
        : gameModel{gameModel},
          messageIO{messageIO},
          commandConfig{commandCreator},
          connectionManager{connectionManager},
          helpCommand{std::make_unique<HelpCommand>(*this)},
          dataRequestHandler{messageIO, commandConfig} {
    registerCommand(COMMAND_HELP, *helpCommand);
}

void Controller::processCommand(const protocols::PlayerCommand& command, const Connection& client) {
    auto cmd = command.command;
    auto cmdArgs = splitString(command.arguments);

    auto accountInfo = getAccountInfo(client);
    auto it = inputToCommandMap.find(cmd);
    if (it == inputToCommandMap.end() || !accountInfo.hasRole(it->second.getRole())) {
        auto msg = DisplayMessageBuilder{"<" + cmd + "> is an invalid command. Type help."}
                .addClient(client)
                .setSender(DisplayMessageBuilder::SENDER_SERVER);
        sendOutput(msg);
        return;
    }

    auto& handler = it->second.getCommand();
    auto playerID = getPlayerID(client);
    auto output = handler.execute(cmdArgs, PlayerInfo{playerID, client});

    sendOutput(*output);
}

void Controller::registerCommand(const std::string& commandId, Command& command) {
    auto commandHandle = commandConfig.createInputBindingsForCommand(commandId, command);
    for (const auto& inputBinding : commandHandle.getInputBindings()) {
        inputToCommandMap.insert({inputBinding, commandHandle});
    }
}

bool Controller::addNewPlayer(const AccountInfo& accountInfo, const networking::Connection& client) {
    if (playerMap.left.count(accountInfo.username) == 1 || playerMap.right.count(client) == 1) {
        return false;
    }

    playerMap.insert(PlayerMapPair{accountInfo.username, client});
    playerAccountMap.insert({accountInfo.username, accountInfo});
    gameModel.createCharacter(accountInfo.username, accountInfo.username);

    auto outMsg = DisplayMessageBuilder{"Player <" +  ColorTag::MAGENTA + accountInfo.username + ColorTag::WHITE + "> has joined."}
            .setSender(DisplayMessageBuilder::SENDER_SERVER)
            .addClients(allClients);
    sendOutput(outMsg);

    allClients.push_back(client);
    return true;
}

const std::vector<Connection>& Controller::getAllClients() const {
    return allClients;
}

GameModel& Controller::getGameModel() const {
    return gameModel;
}

void Controller::removePlayer(const networking::Connection& clientID) {
    if(playerMap.right.count(clientID) == 0) {
        return;
    }

    auto player = getPlayerID(clientID);

    playerMap.right.erase(clientID);
    playerAccountMap.erase(player);
    allClients.erase(std::remove(allClients.begin(), allClients.end(), clientID), allClients.end());
    //TODO remove from game model

    auto outMsg = DisplayMessageBuilder{"Player <" + ColorTag::MAGENTA + player + ColorTag::WHITE + "> has disconnected."}
            .setSender(DisplayMessageBuilder::SENDER_SERVER)
            .addClients(allClients);
    sendOutput(outMsg);
}

boost::optional<Connection> Controller::getClientID(const std::string& playerID) const {
    auto playerMapIterator = playerMap.left.find(playerID);
    bool playerNotFound = playerMapIterator == playerMap.left.end();
    return (playerNotFound) ? boost::optional<Connection>{} : playerMapIterator->second;
}

std::string Controller::getPlayerID(const networking::Connection& clientID) const {
    return playerMap.right.find(clientID)->second;
}

void Controller::disconnectPlayer(const std::string& playerID) {
    auto clientId = getClientID(playerID);
    CHECK(clientId) << "Player does not have client id associated with it";
    connectionManager.disconnectClient(clientId.get());
}

void Controller::update() {
	for(auto& client: getAllClients()) {
		auto targetChar = gameModel.getCharacterByID(getPlayerID(client));
		auto outputBuffer = targetChar->getOutputBuffer();
		for (auto& message: *outputBuffer ) {
			auto displayMessage = DisplayMessageBuilder{message.text}.
					addClient(client).
					setSender(message.color + message.senderID + ColorTag::WHITE);
			sendOutput(displayMessage);
		}
		outputBuffer->clear();
	}
}

void Controller::sendOutput(const MessageBuilder& messageBuilder) const {
    auto msg = PigLatinDecorator{messageBuilder, *this, gameModel};
    messageIO.send(msg);
}

const AccountInfo& Controller::getAccountInfo(const networking::Connection& client) const {
    auto id = getPlayerID(client);
    return playerAccountMap.find(id)->second;
}

void Controller::processCommandInfoRequest(protocols::CommandName cmdInfoRequest, const networking::Connection& client) {
    dataRequestHandler.handleDataRequest(cmdInfoRequest, client);
}