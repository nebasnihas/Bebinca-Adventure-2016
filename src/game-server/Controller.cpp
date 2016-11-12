#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <game/protocols/DisplayMessage.hpp>
#include <glog/logging.h>
#include <unordered_set>
#include "Controller.hpp"
#include "Authenticator.hpp"
#include "StringUtils.hpp"
#include "commands/DisplayMessageBuilder.hpp"
#include "CommandList.hpp"
#include "PigLatinDecorator.hpp"
#include "ConnectionManager.hpp"

using namespace networking;
using namespace std::placeholders;

/*
 * Help command class
 */
class Controller::HelpCommand : public Command {
public:
    HelpCommand(Controller& controller) : controller{controller} {};
    ~HelpCommand() = default;

    virtual std::unique_ptr<MessageBuilder> execute(const gsl::span<std::string, -1> arguments,
                                                    const PlayerInfo& player) override;
private:
    Controller& controller;

    std::unique_ptr<MessageBuilder> allCommandsHelp(const networking::Connection& clientID);
    std::string getCommandBindingsHelpMessage(const std::string& command);
};

std::unique_ptr<MessageBuilder> Controller::HelpCommand::execute(const gsl::span<std::string, -1> arguments,
                                                                 const PlayerInfo& player) {
    if (arguments.empty()) {
        return allCommandsHelp(player.clientID);
    }

    std::string message;
    auto command = arguments[0];

    auto it = controller.inputToCommandMap.find(command);
    if (it == controller.inputToCommandMap.end()) {
        message = "Command <" + command + "> not found.";
    } else {
        message = "Help for command <" + command + ">\n";
        message += "\tDescription: " + it->second.getDescription() + "\n";
        message += "\tUsage: " + command + " " + it->second.getUsage() + "\n";
        message += "\t" + getCommandBindingsHelpMessage(command);
    }

    return DisplayMessageBuilder{message}.addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}

std::unique_ptr<MessageBuilder> Controller::HelpCommand::allCommandsHelp(const networking::Connection& clientID) {
    //Get all unique commands
    std::unordered_set<std::string> commands;
    for (const auto& it : controller.inputToCommandMap) {
        commands.insert(it.second.getId());
    }

    std::string message = "List of available commands. Type help <command> for more information\n";
    for (const auto& cmd : commands) {
        message += "Name: " + cmd;
        message += " - " + getCommandBindingsHelpMessage(cmd);
        message += "\n";
    }

    return DisplayMessageBuilder{message}.addClient(clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}

std::string Controller::HelpCommand::getCommandBindingsHelpMessage(const std::string& command) {
    std::string message;
    message += "Command:[";
    message += boost::algorithm::join(controller.inputToCommandMap.at(command).getInputBindings(), ",");
    message += "]";

    return message;
}

/*
 * Controller
 */

Controller::Controller(GameModel& gameModel, MessageIO& messageIO, ConnectionManager& connectionManager,
                       const CommandConfig& commandCreator)
        : gameModel{gameModel},
          messageIO{messageIO},
          commandConfig{commandCreator},
          connectionManager{connectionManager},
          helpCommand{std::make_unique<HelpCommand>(*this)} {
    registerCommand(COMMAND_HELP, *helpCommand);

}

static constexpr bool hasRole(int roleFlags, PlayerRole role) {
    return (bool)(roleFlags & (int)role);
}

void Controller::processCommand(const protocols::PlayerCommand& command,
                                const Connection& client) {
    auto cmd = command.command;
    auto cmdArgs = splitString(command.arguments);

    auto accountInfo = getAccountInfo(client);
    auto it = inputToCommandMap.find(cmd);
    if (it == inputToCommandMap.end() || !hasRole(accountInfo.playerRoleFlags, it->second.getRole())) {
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
    allClients.push_back(client);
    gameModel.createCharacter(accountInfo.username, accountInfo.username);

    auto outMsg = DisplayMessageBuilder{"Player <" + accountInfo.username + "> has joined."}
            .setSender(DisplayMessageBuilder::SENDER_SERVER)
            .addClients(allClients);
    sendOutput(outMsg);

    return true;
}

const std::vector<Connection>& Controller::getAllClients() const {
    return allClients;
}

GameModel& Controller::getGameModel() const {
    return gameModel;
}

void Controller::removePlayer(const networking::Connection& clientID) {
    auto player = getPlayerID(clientID);

    playerMap.right.erase(clientID);
    playerAccountMap.erase(player);
    allClients.erase(std::remove(allClients.begin(), allClients.end(), clientID), allClients.end());
    //TODO remove from game model

    auto outMsg = DisplayMessageBuilder{"Player <" + player + "> has disconnected."}
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
			auto displayMessage = DisplayMessageBuilder{message}.
					addClient(client).
					setSender(DisplayMessageBuilder::SENDER_SERVER);
			sendOutput(displayMessage);
		}
		outputBuffer->clear();
	}
}

void Controller::sendOutput(const MessageBuilder& messageBuilder) const {
    auto msg = PigLatinDecorator{messageBuilder};
    messageIO.send(msg);
}

const AccountInfo& Controller::getAccountInfo(const networking::Connection& client) const {
    auto id = getPlayerID(client);
    return playerAccountMap.find(id)->second;
}





