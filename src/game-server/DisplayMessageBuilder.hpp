#ifndef ADVENTURE2016_MESSAGEBUILDER_HPP
#define ADVENTURE2016_MESSAGEBUILDER_HPP

#include <boost/optional.hpp>
#include "networking/server.h"
#include "game/protocols/DisplayMessage.hpp"

using namespace networking;

class DisplayMessageBuilder {
public:
    static const std::string SENDER_SERVER;
    static DisplayMessageBuilder createMessage(const std::string& message);

    DisplayMessageBuilder& addClient(const Connection& client);
    DisplayMessageBuilder& addClients(const std::vector<Connection>& clients);
    DisplayMessageBuilder& addClients(const std::initializer_list<Connection>& clients);
    DisplayMessageBuilder& setSender(const std::string& sender);

    std::vector<networking::Message> getOutputMessages() const;
private:
    std::vector<Connection> clientList;
    std::string message;
    boost::optional<std::string> sender;

    DisplayMessageBuilder(const std::string& message) : message{message} {};
};


#endif //ADVENTURE2016_MESSAGEBUILDER_HPP
