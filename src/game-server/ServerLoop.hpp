#ifndef ADVENTURE2016_GAMELOOP_HPP
#define ADVENTURE2016_GAMELOOP_HPP

#include "networking/server.h"
#include "Controller.hpp"
#include "game/protocols/RequestMessage.hpp"
#include "game/protocols/Authentication.hpp"
#include "game/GameDataImporter.hpp"
#include "Authenticator.hpp"
#include "GameFunctions.hpp"
#include "glog/logging.h"
#include "Looper.hpp"

class ServerLoop : public Loop {
public:
    //TODO use config file
    ServerLoop(unsigned short serverPort, const std::string& mapFilePath);

    virtual void processInputs(bool& quit) override;
    virtual void update() override;

private:
    networking::Server server;
    GameModel gameModel;
    Controller controller;
    GameFunctions gameFunctions;

    void onConnect(networking::Connection c);
    void onDisconnect(networking::Connection c);
    void processLoginRequest(const protocols::RequestMessage& request, const networking::Connection& clientId);
    void processRegistrationRequest(const protocols::RequestMessage& request, const networking::Connection& clientId);
};


#endif //ADVENTURE2016_GAMELOOP_HPP
