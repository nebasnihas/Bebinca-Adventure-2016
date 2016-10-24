#ifndef ADVENTURE2016_DISPLAYMESSAGEBUILDER_HPP
#define ADVENTURE2016_DISPLAYMESSAGEBUILDER_HPP

#include <boost/optional.hpp>
#include <memory>
#include "networking/server.h"
#include "game/protocols/DisplayMessage.hpp"
#include "MessageBuilder.hpp"

/*
 * Builds messages that will be displayed on the client
 */
class DisplayMessageBuilder : public MessageBuilder {
public:
    static const std::string SENDER_SERVER;
    static DisplayMessageBuilder createMessage(const std::string& message);
    DisplayMessageBuilder(const std::string& message) : message{message} {};

    operator std::unique_ptr<MessageBuilder>();

    DisplayMessageBuilder& addClient(const networking::Connection& client);
    DisplayMessageBuilder& addClients(const std::vector<networking::Connection>& clients);
    DisplayMessageBuilder& addClients(const std::initializer_list<networking::Connection>& clients);
    DisplayMessageBuilder& setSender(const std::string& sender);

    virtual std::vector<networking::Message> buildMessages() const override ;
private:
    std::vector<networking::Connection> clientList;
    std::string message;
    boost::optional<std::string> sender;
};


#endif //ADVENTURE2016_DISPLAYMESSAGEBUILDER_HPP
