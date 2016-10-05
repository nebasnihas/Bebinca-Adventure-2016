#include "DisplayMessageBuilder.hpp"

const std::string DisplayMessageBuilder::SENDER_SERVER = "Server"; //TODO reserve default name

DisplayMessageBuilder& DisplayMessageBuilder::addClient(const Connection& client)
{
    clientList.push_back(client);

    return *this;
}

DisplayMessageBuilder& DisplayMessageBuilder::addClients(const std::vector<Connection>& clients)
{
    clientList.reserve(clientList.size() + clients.size());
    clientList.insert(clientList.end(), clients.begin(), clients.end());

    return *this;
}

DisplayMessageBuilder& DisplayMessageBuilder::addClients(const std::initializer_list<Connection>& clients)
{
    clientList.reserve(clientList.size() + clients.size());
    clientList.insert(clientList.end(), clients.begin(), clients.end());

    return *this;
}

DisplayMessageBuilder& DisplayMessageBuilder::setSender(const std::string& sender)
{
    this->sender.emplace(sender);
    return *this;
}

DisplayMessageBuilder DisplayMessageBuilder::createMessage(const std::string& message)
{
    return DisplayMessageBuilder(message);
}

std::vector<networking::Message> DisplayMessageBuilder::getOutputMessages() const
{
    std::vector<networking::Message> output(clientList.size());

    for (const auto& client : clientList) {
        auto messageForClient = protocols::DisplayMessage{message, sender};
        auto responseMessage = protocols::createDisplayResponseMessage(messageForClient);
        auto serializedResponseMessage = protocols::serializeResponseMessage(responseMessage);

        output.push_back(Message{client, serializedResponseMessage});
    }

    return output;
}
