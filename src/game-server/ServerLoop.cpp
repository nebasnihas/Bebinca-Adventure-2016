#include <functional>
#include <iostream>
#include "ServerLoop.hpp"

using namespace networking;

ServerLoop::ServerLoop(const ServerConfig& serverConfig)
        : serverConfig{serverConfig},
          server{serverConfig.getPort(), [this](Connection c){this->onConnect(c);}, [this](Connection c){this->onDisconnect(c);}},
          controller{gameModel, server, CommandCreator{serverConfig.getCommandConfigFile()}},
          gameFunctions{controller} {
    GameDataImporter::loadyamlFile(gameModel, serverConfig.getMapFilePath());
    std::cout << "Server ready. Listening on port: " << serverConfig.getPort() << std::endl;
}

void ServerLoop::processInputs(bool& quit) {
    try {
        server.update();
    } catch (std::exception& e) {
        LOG(ERROR) << "Exception from Server update:\n" << e.what();
        quit = true;
    }
}

void ServerLoop::update() {
    auto incoming = server.receive();
    for (const auto &clientMessage : incoming) {
        auto requestMessage = protocols::deserializeRequestMessage(clientMessage.text);

        switch (requestMessage.header) {
            case protocols::RequestHeader::LOGIN_REQUEST:
                processLoginRequest(requestMessage, clientMessage.connection);
                break;
            case protocols::RequestHeader::REGISTER_REQUEST:
                processRegistrationRequest(requestMessage, clientMessage.connection);
                break;
            case protocols::RequestHeader::PLAYER_COMMAND_REQUEST: {
                auto playerCommand = protocols::readPlayerCommandRequestMessage(requestMessage);
                auto outgoing = controller.processCommand(playerCommand, clientMessage.connection)->buildMessages();
                server.send(outgoing);
                break;
            }
            default:
                break;
        }
    }
}

void ServerLoop::onConnect(Connection c) {
    LOG(INFO) << "New connection found: " << c.id;
}

void ServerLoop::onDisconnect(Connection c) {
    LOG(INFO) << "Connection lost: " << c.id;
    controller.removePlayer(c);
}

void ServerLoop::processLoginRequest(const protocols::RequestMessage& request, const Connection& clientId) {
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

void ServerLoop::processRegistrationRequest(const protocols::RequestMessage& request, const Connection& clientId) {
    auto registrationRequest = protocols::readAuthenticationRequestMessage(request);
    auto responseCode = Authenticator::registerAccount(registrationRequest.username, registrationRequest.password);
    auto registrationResponse = protocols::createRegistrationResponseMessage(responseCode);

    auto output = protocols::serializeResponseMessage(registrationResponse);
    server.send(Message{clientId, output});

    if (responseCode == protocols::RegistrationResponseCode::REGISTRATION_OK) {
        controller.addNewPlayer(PlayerInfo{registrationRequest.username, clientId});
    }
}