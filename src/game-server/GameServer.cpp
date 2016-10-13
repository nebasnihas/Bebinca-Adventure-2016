/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "networking/server.h"
#include "Controller.hpp"
#include "game/protocols/RequestMessage.hpp"
#include "game/protocols/Authentication.hpp"
#include "Authenticator.hpp"
#include "GameFunctions.hpp"

#include <boost/lexical_cast.hpp>
#include <queue>

using namespace networking;
using namespace protocols;

std::vector<Connection> clients;
GameModel gameModel;
Controller controller{gameModel, clients};
GameFunctions gameFunctions{controller};

void onConnect(Connection connection) {
    printf("New connection found: %lu\n", connection.id);
    clients.push_back(connection);
}


void onDisconnect(Connection c) {
    printf("Connection lost: %lu\n", c.id);
    auto it = std::remove(clients.begin(), clients.end(), c);
    clients.erase(it, clients.end());
    //TODO remove player from game
}

bool validateServerArgs(int argc, char* argv[], unsigned short& port) {
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

void processLoginRequest(const protocols::RequestMessage& request, const Connection& clientId, Server& server) {
    auto loginRequest = protocols::readAuthenticationRequestMessage(request);
    auto responseCode = Authenticator::login(loginRequest.username, loginRequest.password);
    auto loginResponse = protocols::createLoginResponseMessage(responseCode);

    auto output = protocols::serializeResponseMessage(loginResponse);
    server.send(Message{clientId, output});

    controller.addNewPlayer(PlayerInfo{loginRequest.username, clientId});
}

void processRegistrationRequest(const protocols::RequestMessage& request, const Connection& clientId, Server& server) {
    auto registrationRequest = protocols::readAuthenticationRequestMessage(request);
    auto responseCode = Authenticator::registerAccount(registrationRequest.username, registrationRequest.password);
    auto registrationResponse = protocols::createRegistrationResponseMessage(responseCode);

    auto output = protocols::serializeResponseMessage(registrationResponse);
    server.send(Message{clientId, output});

    controller.addNewPlayer(PlayerInfo{registrationRequest.username, clientId});
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
            auto requestMessage = protocols::deserializeRequestMessage(clientMessage.text);

            switch (requestMessage.header) {
                case protocols::RequestHeader::LOGIN_REQUEST:
                    processLoginRequest(requestMessage, clientMessage.connection, server);
                    break;
                case protocols::RequestHeader::REGISTER_REQUEST:
                    processRegistrationRequest(requestMessage, clientMessage.connection, server);
                    break;
                case protocols::RequestHeader::PLAYER_COMMAND_REQUEST: {
                    auto playerCommand = protocols::readPlayerCommandRequestMessage(requestMessage);
                    auto outgoing = controller.processCommand(playerCommand, clientMessage.connection);
                    server.send(outgoing.getOutputMessages());
                    break;
                }
                default:
                    break;
            }
        }


        sleep(1);
    }

    return 0;
}

