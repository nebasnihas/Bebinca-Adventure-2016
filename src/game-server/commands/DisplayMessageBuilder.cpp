#include "DisplayMessageBuilder.hpp"

const std::string DisplayMessageBuilder::SENDER_SERVER = "Server"; //TODO reserve default name

DisplayMessageBuilder& DisplayMessageBuilder::addClient(const networking::Connection& client)
{
    clientList.push_back(client);

    return *this;
}

DisplayMessageBuilder& DisplayMessageBuilder::addClients(const gsl::span<networking::Connection, -1> clients)
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

std::vector<networking::Message> DisplayMessageBuilder::buildMessages() const
{
    std::vector<networking::Message> output;
    output.reserve(clientList.size());

    for (const auto& client : clientList) {
        auto messageForClient = protocols::DisplayMessage{message, sender};
        auto responseMessage = protocols::createDisplayResponseMessage(messageForClient);
        auto serializedResponseMessage = protocols::serializeResponseMessage(responseMessage);

        output.push_back(networking::Message{client, serializedResponseMessage});
    }

    return output;
}

DisplayMessageBuilder::operator std::unique_ptr<MessageBuilder>() {
    return std::make_unique<DisplayMessageBuilder>(*this);
}
