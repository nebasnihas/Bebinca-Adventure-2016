#ifndef GAMESERVER_MESSAGESENDER_H
#define GAMESERVER_MESSAGESENDER_H

#include <string>
#include <deque>
#include <vector>

#include "server.h"

using namespace networking;

static std::string SYSTEM_SENDER =  "System";

class MessageSender {
public:
    MessageSender(Server& server, const std::vector<Connection>& clients) : server{server}, clients{clients} {};

    void sendMessage(const std::string& message);
    void sendMessage(const std::string& message, const Connection& receiver);
    void sendMessage(const std::string& message, const std::string& sender);
    void sendMessage(const std::string& message, const std::string& sender, const Connection& receiver);
private:
    const std::vector<Connection>& clients;
    Server& server;
};


#endif
