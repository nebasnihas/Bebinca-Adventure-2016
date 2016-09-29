#include <clocale>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "Controller.hpp"

using namespace std::placeholders;

std::vector<std::string> splitArgs(const std::string& args)
{
    std::vector<std::string> tokens;
    boost::split(tokens, args, boost::is_any_of(" \t"));
    return tokens;
}

void Controller::processCommand(const std::string& message, const Connection& connectionID, MessageSender& messageSender) {
    std::string delim = " ";
    auto delimPos = message.find(delim);

    std::string command;
    std::string args;

    if (delimPos == std::string::npos) {
        command = message;
    } else {
        command = message.substr(0, delimPos);
        args = message.substr(delimPos + 1);
    }

    executeCommand(command, splitArgs(args), connectionID, messageSender);
}


std::string Controller::executeCommand(const std::string& command, const std::vector<std::string>& targets,
                                       const Connection& connection, MessageSender& messageSender) {

    Command::functionRef targetMethod;
    try {
        targetMethod = commandMap[command].getMethod();
//        targetMethod(connectionToPlayerMap[connection], targets, messageSender);
    }
    catch (const std::out_of_range& oor) {
        return "Command not found";
    }
}

Controller::Controller(GameModel& gameModel) {
    this->gameModel = gameModel;

    auto target = &Controller::look;
    Command::functionRef callLook = std::bind(target, this, _1, _2, _3);
    registerCommand(Command::Type::AREA, string("look"), callLook);
}

void Controller::registerCommand(Command::Type type, std::string commandText, Command::functionRef method) {
    Command command{type, commandText, method};
    commandMap.insert(std::make_pair(commandText, command));
}

std::string Controller::look(int playerID, vector<string> targets, MessageSender& messageSender) {
    auto character = gameModel.getCharacterByID(playerID);
    std::string areaDescription = gameModel.getAreaDescription(character->getAreaID());
    messageSender.sendMessage(areaDescription, playerToConnectionMap[playerID]);
    return areaDescription;
}

std::string Controller::authHandler(const std::string& args, const Connection& client, MessageSender& messageSender)
{
    //TODO placeholder. args are user and password separated by space
    auto credentials = splitArgs(args);
    if (credentials.size() != 2 || credentials[1] != "password") {
        messageSender.sendMessage("auth:bad", client);
    } else {
        std::string name = credentials[0];
//        clientMap[client] = name;


        messageSender.sendMessage(name + " has joined\n", SYSTEM_SENDER);
        messageSender.sendMessage("auth:ok", client);
    }
    return nullptr;
}


