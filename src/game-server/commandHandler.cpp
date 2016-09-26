#include "commandHandler.h"

#include <map>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <iostream>

std::vector<std::string> splitArgs(const std::string& args)
{
    std::vector<std::string> tokens;
    boost::split(tokens, args, boost::is_any_of(" \t"));
    return tokens;
}

std::unordered_map<Connection, std::string, ConnectionHash> clientMap; //this is supposed to be implemented in the game model. because the game will deal with mapping a client to its representation

void authHandler(const std::string& args, const Connection& client, MessageSender& messageSender)
{
    //TODO placeholder. args are user and password separated by space
    auto credentials = splitArgs(args);
    if (credentials.size() != 2 || credentials[1] != "password") {
        messageSender.sendMessage("auth:bad", client);
    } else {
        std::string name = credentials[0];
        clientMap[client] = name;

        messageSender.sendMessage(name + " has joined\n", SYSTEM_SENDER);
        messageSender.sendMessage("auth:ok", client);
    }
}

void sayHandler(const std::string& args, const Connection& client, MessageSender& messageSender)
{
    std::string message = args + "\n";
    messageSender.sendMessage(message, clientMap[client]);
}


namespace
{
    std::unordered_map<std::string, std::function<void(const std::string& args,
                                                       const Connection& client,
                                                       MessageSender& messageSender)>> handlerMap = {
            {"say", &sayHandler},
            {"auth", &authHandler}
    };
}

void processCommand(const Message& message, MessageSender& messageSender)
{
    //TODO decide on message format/protocol, for now the first word is the command and the rest is the argument

    std::string delim = " ";
    auto delimPos = message.text.find(delim);

    std::string command;
    std::string args;

    if (delimPos == std::string::npos) {
        command = message.text;
    } else {
        command = message.text.substr(0, delimPos);
        args = message.text.substr(delimPos + 1);
    }

    if (handlerMap.count(command) == 0) {
        messageSender.sendMessage("Unknown command: " + message.text + "\n", SYSTEM_SENDER, message.connection);
    } else {
        handlerMap[command](args, message.connection, messageSender);
    }
}
