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
#include "game/GameDataImporter.hpp"
#include "Authenticator.hpp"
#include "GameFunctions.hpp"
#include "glog/logging.h"

#include <boost/lexical_cast.hpp>
#include <queue>

using namespace networking;
using namespace protocols;

void onConnect(Connection c) {
    LOG(INFO) << "New connection found: " << c.id;
}

//TODO find better way to disconnect
std::vector<networking::Connection> disconnectedClients;
void onDisconnect(Connection c) {
    LOG(INFO) << "Connection lost: " << c.id;
    disconnectedClients.push_back(c);
}

void validateServerArgs(int argc, char* argv[], unsigned short& port, std::string& filename) {
    if (argc < 3) {
        std::cout << "Usage:\n" << argv[0] << " <port> <pathToMap>\ne.g. " << argv[0] << "4002 mgoose.yml\n" << std::endl;
        exit(1);
    }

    try {
        port = boost::lexical_cast<ushort>(argv[1]);
    } catch (const boost::bad_lexical_cast&) {
        std::cerr << "Invalid port number" << std::endl;
        exit(1);
    }

    filename = std::string(argv[2]);
}

void processLoginRequest(const protocols::RequestMessage& request, const Connection& clientId, Server& server, Controller& controller) {
    auto loginRequest = protocols::readAuthenticationRequestMessage(request);
    auto responseCode = Authenticator::login(loginRequest.username, loginRequest.password);
    auto loginResponse = protocols::createLoginResponseMessage(responseCode);

    auto output = protocols::serializeResponseMessage(loginResponse);
    server.send(Message{clientId, output});

    //TODO character select. for now skip it and add the player after sucess
    if (responseCode == protocols::LoginResponseCode::LOGIN_OK) {
        controller.addNewPlayer(PlayerInfo{loginRequest.username, clientId});
    }
}

void processRegistrationRequest(const protocols::RequestMessage& request, const Connection& clientId, Server& server, Controller& controller) {
    auto registrationRequest = protocols::readAuthenticationRequestMessage(request);
    auto responseCode = Authenticator::registerAccount(registrationRequest.username, registrationRequest.password);
    auto registrationResponse = protocols::createRegistrationResponseMessage(responseCode);

    auto output = protocols::serializeResponseMessage(registrationResponse);
    server.send(Message{clientId, output});

    if (responseCode == protocols::RegistrationResponseCode::REGISTRATION_OK) {
        controller.addNewPlayer(PlayerInfo{registrationRequest.username, clientId});
    }
}

int main(int argc, char *argv[]) {
    //TODO configuration and arg checking
    FLAGS_log_dir = "./";
    google::InitGoogleLogging("GameServer");
    unsigned short port;
    std::string sourceFile;
    validateServerArgs(argc, argv, port, sourceFile);

    //Setup server
    Server server{port, onConnect, onDisconnect};

    //Setup game
    GameModel gameModel;
    GameDataImporter::loadyamlFile(gameModel, sourceFile);
    Controller controller{gameModel, server};
    GameFunctions gameFunctions{controller};

    bool done = false;
    while (!done) {
        try {
            server.update();
        } catch (std::exception &e) {
            LOG(ERROR) << "Exception from Server update:\n" << e.what();
            done = true;
        }

        auto incoming = server.receive();
        for (const auto &clientMessage : incoming) {
            auto requestMessage = protocols::deserializeRequestMessage(clientMessage.text);

            switch (requestMessage.header) {
                case protocols::RequestHeader::LOGIN_REQUEST:
                    processLoginRequest(requestMessage, clientMessage.connection, server, controller);
                    break;
                case protocols::RequestHeader::REGISTER_REQUEST:
                    processRegistrationRequest(requestMessage, clientMessage.connection, server, controller);
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

        for (const auto& client: disconnectedClients) {
            controller.removePlayer(client);
        }
        disconnectedClients.clear();


        sleep(1);
    }

    return 0;
}

