/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "networking/server.h"
#include "BufferedMessageSender.h"
#include "Controller.hpp"
#include "game/protocols/Message.hpp"
#include "game/protocols/Authentication.hpp"
#include "Authenticator.hpp"

#include <boost/lexical_cast.hpp>
#include <queue>

using namespace networking;
using namespace protocols;

std::vector<Connection> clients;
BufferedMessageSender messageSender;
GameModel gameModel;
Controller controller;

void onConnect(Connection connection) {
    printf("New connection found: %lu\n", connection.id);
    clients.push_back(connection);
}


void onDisconnect(Connection c) {
    printf("Connection lost: %lu\n", c.id);
    auto it = std::remove(clients.begin(), clients.end(), c);
    clients.erase(it, clients.end());
}

bool validateServerArgs(int argc, char* argv[], unsigned short& port)
{
    if (argc < 2) {
        printf("Usage:\n%s <port>\ne.g. %s 4002\n", argv[0], argv[0]);
        return false;
    }

    try {
        port = boost::lexical_cast<ushort>(argv[1]);
    } catch (const boost::bad_lexical_cast&) {
        std::cout << "Invalid port number" << std::endl;
        return false;
    }

    return true;
}

void processLoginRequest(const std::string& username, const std::string& password, Connection clientId, Server& server)
{
    std::string response;
    switch (Authenticator::login(username, password)) {
        case LoginStatus::INVALID_CREDENTIALS:
            response = createAuthResponseMessage(AuthType::LOGIN, false, "The credentials do not match\n");
            break;
        case LoginStatus::USERNAME_NOT_FOUND:
            response = createAuthResponseMessage(AuthType::LOGIN, false, "Username not found\n");
            break;
        case LoginStatus::OK:
            response = createAuthResponseMessage(AuthType::LOGIN, true, username);
            break;
    }

    messageSender.sendMessage(createMessage(MessageType::AUTHENTICATION, response), clientId);
    messageSender.sendAll(server, clients);
}

void processRegistrationRequest(const std::string& username, const std::string password, Connection clientId, Server& server)
{
    std::string response;
    switch (Authenticator::registerAccount(username, password)) {
        case RegistrationStatus::USERNAME_EXISTS:
            response = createAuthResponseMessage(AuthType::LOGIN, false, "Username already exists");
            break;
        case RegistrationStatus::USERNAME_TOO_LONG:
            response = createAuthResponseMessage(AuthType::LOGIN, false, "Username is too long. max: "
                                                                         + std::to_string(Authenticator::USERNAME_MAX_LENGTH) + "\n");
            break;
        case RegistrationStatus::OK:
            response = createAuthResponseMessage(AuthType::LOGIN, true, username);
            break;
    }

    messageSender.sendMessage(createMessage(MessageType::AUTHENTICATION, response), clientId);
    messageSender.sendAll(server, clients);
}

void processAuthenicationRequest(const std::string& request, Connection clientId, Server& server)
{
    auto msg = readAuthRequestMessage(request);

    if (msg.authType == AuthType::LOGIN) {
        processLoginRequest(msg.userName, msg.password, clientId, server);
    } else if (msg.authType == AuthType::REGISTER) {
        processRegistrationRequest(msg.userName, msg.password, clientId, server);
    }
}

int main(int argc, char *argv[]) {
    unsigned short port;
    if (!validateServerArgs(argc, argv, port))
    {
        return 1;
    }

    Server server{port, onConnect, onDisconnect};

    bool done = false;
    while (!done) {
        try {
            server.update();
        } catch (std::exception &e) {
            printf("Exception from Server update:\n%s\n\n", e.what());
            done = true;
        }

        auto incoming = server.receive();
        for (const auto &clientMessage : incoming) {
            auto gameMessage = protocols::readMessage(clientMessage.text);
            if (gameMessage.type == protocols::MessageType::AUTHENTICATION) {
                processAuthenicationRequest(gameMessage.messageBody, clientMessage.connection, server);
            } else {
                controller.processCommand(clientMessage.text, clientMessage.connection, gameModel, messageSender);
            }
        }

        messageSender.sendAll(server, clients);

        sleep(1);
    }

    return 0;
}

