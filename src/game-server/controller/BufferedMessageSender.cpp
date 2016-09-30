#include "BufferedMessageSender.h"

void BufferedMessageSender::sendMessage(const std::string& message, const std::string& sender)
{

    messageBuffer.push_back(OutMsg{boost::optional<const Connection>{}, MessageSender::formatMessage(message, sender)});
}

void BufferedMessageSender::sendMessage(const std::string& message, const std::string& sender, const Connection &receiver)
{
    messageBuffer.push_back(OutMsg{boost::optional<const Connection>{receiver}, MessageSender::formatMessage(message, sender)});
}

void BufferedMessageSender::sendMessage(const std::string &message)
{
    messageBuffer.push_back(OutMsg{boost::optional<const Connection>{}, message});
}

void BufferedMessageSender::sendMessage(const std::string &message, const Connection &receiver)
{
    messageBuffer.push_back(OutMsg{boost::optional<const Connection>{receiver}, message});
}

void BufferedMessageSender::sendAll(Server &server, const std::vector<Connection> &clientList) {
    for (const auto& msg : messageBuffer) {
        if (msg.client) {
            server.send(Message{*(msg.client), msg.message});
        } else {
            for (const auto& client : clientList) {
                server.send(Message{client, msg.message});
            }
        }
    }

    messageBuffer.clear();
}

