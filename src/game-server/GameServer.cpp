/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "networking/server.h"
#include "BufferedMessageSender.h"
#include "Controller.hpp"

#include <boost/lexical_cast.hpp>
#include <queue>

using namespace networking;

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
        for (auto &message : incoming) {
            if (message.text == "quit") {
                server.disconnect(message.connection);
            } else if (message.text == "shutdown") {
                printf("Shutting down.\n");
                done = true;
            } else {
                controller.processCommand(message.text, message.connection, gameModel, messageSender);
            }
        }

        messageSender.sendAll(server, clients);

        sleep(1);
    }

    return 0;
}

