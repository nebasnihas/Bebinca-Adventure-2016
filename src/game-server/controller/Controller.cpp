#include <clocale>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "Controller.hpp"

using namespace networking;
using namespace std::placeholders;

std::vector<std::string> splitArgs(const std::string& args)
{
    std::vector<std::string> tokens;
    boost::split(tokens, args, boost::is_any_of(" \t"));
    return tokens;
}

Controller::Controller() {
    registerCommand(Command::Type::AREA, "look", std::bind(&Controller::look, this, _1, _2, _3, _4, _5));
    registerCommand(Command::Type::GAME, "auth", std::bind(&Controller::authHandler, this, _1, _2, _3, _4, _5));
    registerCommand(Command::Type::GAME, "say", std::bind(&Controller::sayHandler, this, _1, _2, _3, _4, _5));
}

void Controller::processCommand(const std::string& message, const Connection& client, GameModel& gameModel, MessageSender& messageSender) {
    const std::string delim = " ";
    auto delimPos = message.find(delim);

    std::string command;
    std::string args;

    if (delimPos == std::string::npos) {
        //there is only the command, no arguments
        command = message;
    } else {
        command = message.substr(0, delimPos);
        args = message.substr(delimPos + 1);
    }

    executeCommand(command, splitArgs(args), client, gameModel, messageSender);
}


void Controller::executeCommand(const std::string& command, const std::vector<std::string>& targets,
                                const Connection& client, GameModel& gameModel, MessageSender& messageSender) {

    int playerId;
    //check if client has a character yet
    try {
        playerId = clientToPlayerMap[client];
    } catch (const std::out_of_range& e) {
        playerId = -1;
    }

    try {
        auto targetMethod = commandMap.at(command).getMethod();
        targetMethod(targets,playerId, client, gameModel, messageSender);
    } catch (const std::out_of_range& oor) {
        messageSender.sendMessage("<" + command + "> is an invalid command.\n", SENDER_DEFAULT, client);
    }
}

void Controller::registerCommand(const Command::Type& type, const std::string& commandText, Command::functionRef method) {
    Command command{type, commandText, method};
    commandMap.insert(std::make_pair(commandText, command));
}

void Controller::look(const std::vector<std::string>& targets, int playerID, const Connection& clientID, GameModel& gameModel, MessageSender& messageSender) {
//    auto character = gameModel.getCharacterByID(playerID);
//    std::string areaDescription = gameModel.getAreaDescription(character->getAreaID());

    messageSender.sendMessage("You look around...\n", SENDER_DEFAULT, clientID);
}

void Controller::authHandler(const std::vector<std::string>& targets, int playerID, const Connection& clientID, GameModel& gameModel, MessageSender& messageSender)
{
    //TODO placeholder. args are user and password separated by space
    if (targets.size() != 2 || targets[1] != "password") {
        messageSender.sendMessage("auth:bad", clientID);
    } else {
        static int id = 0;
        clientToPlayerMap[clientID] = ++id;
        playerToClientMap[id] = clientID;

        messageSender.sendMessage("player id: " + std::to_string(id) + " has joined\n", SENDER_DEFAULT);
        messageSender.sendMessage("auth:ok", clientID);
    }
}

void Controller::sayHandler(const std::vector<std::string> &targets, int playerID, const Connection &clientID,
                            GameModel &gameModel, MessageSender &messageSender)
{
    std::string str;
    for (const auto& s : targets) {
        str += s;
    }
    messageSender.sendMessage(str + "\n", std::to_string(playerID));

}


