#include "messageSender.h"

void MessageSender::sendMessage(const std::string& message, const std::string& sender)
{
    for (const auto& client : clients) {
        sendMessage(message, sender, client);
    }
}

void MessageSender::sendMessage(const std::string& message, const std::string& sender, const Connection &receiver)
{
    std::string formattedMsg = "[" + sender + "] > " + message;
    sendMessage(formattedMsg, receiver);
}

void MessageSender::sendMessage(const std::string &message)
{
    for (const auto& client : clients) {
        sendMessage(message, client);
    }
}

void MessageSender::sendMessage(const std::string &message, const Connection &receiver)
{
    server.send(Message{receiver, message});
}