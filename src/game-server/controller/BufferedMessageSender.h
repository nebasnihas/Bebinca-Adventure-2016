#ifndef ADVENTURE2016_BUFFEREDMESSAGESENDER_H
#define ADVENTURE2016_BUFFEREDMESSAGESENDER_H

#include <string>

#include "MessageSender.h"
#include "boost/optional.hpp"
#include "server.h"

using namespace networking;

class BufferedMessageSender : public MessageSender {
public:
    void sendAll(Server& server, const std::vector<Connection>& clientList);

    virtual void sendMessage(const std::string& message) override;
    virtual void sendMessage(const std::string& message, const Connection& receiver) override;
    virtual void sendMessage(const std::string& message, const std::string& sender) override;
    virtual void sendMessage(const std::string& message, const std::string& sender, const Connection& receiver) override;

private:
    struct OutMsg {
        boost::optional<const Connection> client;
        std::string message;
    };

    std::vector<OutMsg> messageBuffer;
};


#endif //ADVENTURE2016_BUFFEREDMESSAGESENDER_H
