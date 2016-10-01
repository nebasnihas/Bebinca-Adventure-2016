#ifndef GAMESERVER_MESSAGESENDER_H
#define GAMESERVER_MESSAGESENDER_H

#include <string>

#include "networking/server.h"

using namespace networking;

#define SENDER_DEFAULT "Server"

class MessageSender {
public:
    virtual ~MessageSender(){};
    virtual void sendMessage(const std::string& message) = 0 ;
    virtual void sendMessage(const std::string& message, const Connection& receiver) = 0;
    virtual void sendMessage(const std::string& message, const std::string& sender) = 0;
    virtual void sendMessage(const std::string& message, const std::string& sender, const Connection& receiver) = 0;

protected:
    static std::string formatMessage(const std::string& message, const std::string& sender) {
        return "[" + sender + "] > " + message;

    }
};


#endif
