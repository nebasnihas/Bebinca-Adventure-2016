/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "networking/client.h"
#include "StringUtils.hpp"
#include "game/protocols/RequestMessage.hpp"
#include "game/protocols/Authentication.hpp"
#include "game/protocols/PlayerCommand.hpp"
#include "game/protocols/DisplayMessage.hpp"
#include "Application.hpp"
#include "MainMenuWindow.hpp"
#include "AuthenticationWindow.hpp"
#include "ChatWindow.hpp"
#include <glog/logging.h>

using namespace networking;

static const std::string MAIN_MENU_WINDOW_ID = "auth";
static const std::string LOGIN_WINDOW_ID = "login";
static const std::string REGISTER_WINDOW_ID = "register";
static const std::string CHAT_WINDOW_ID = "chat";
bool running = true;

std::unique_ptr<Client> networkingClient;
std::unique_ptr<gui::Application> gameClient;
std::unique_ptr<gui::MainMenuWindow> authWindow;
std::unique_ptr<gui::AuthenticationWindow> loginWindow;
std::unique_ptr<gui::AuthenticationWindow> registerWindow;
std::unique_ptr<gui::ChatWindow> chatWindow;

void setupAuthWindow() {
    authWindow = std::make_unique<gui::MainMenuWindow>();
    authWindow->setOnSelection([](auto selection){
        switch(selection) {
            case gui::MainMenuChoice::LOGIN:
                gameClient->switchToWindow(LOGIN_WINDOW_ID);
                break;
            case gui::MainMenuChoice::REGISTER:
                gameClient->switchToWindow(REGISTER_WINDOW_ID);
                break;
            case gui::MainMenuChoice::EXIT:
                running = false;
                break;
            default:
                LOG(FATAL) << "No case for choice";
        }
    });

    gameClient->addWindow(MAIN_MENU_WINDOW_ID, authWindow.get());
}

void setupLoginWindow() {
    loginWindow = std::make_unique<gui::AuthenticationWindow>("LOGIN");
    loginWindow->setOnCancel([](){
        gameClient->switchToWindow(MAIN_MENU_WINDOW_ID);
    });
    loginWindow->setOnInput([](auto user, auto password) {
        auto requestMessage = protocols::createLoginRequestMessage({user, password});
        networkingClient->send(protocols::serializeRequestMessage(requestMessage));
    });

    gameClient->addWindow(LOGIN_WINDOW_ID, loginWindow.get());
}

void setupRegisterWindow() {
    registerWindow = std::make_unique<gui::AuthenticationWindow>("REGISTER");
    registerWindow->setOnCancel([](){
        gameClient->switchToWindow(MAIN_MENU_WINDOW_ID);
    });
    registerWindow->setOnInput([](auto username, auto password) {
        auto requestMessage = protocols::createRegistrationRequestMessage({username, password});
        networkingClient->send(protocols::serializeRequestMessage(requestMessage));
    });

    gameClient->addWindow(REGISTER_WINDOW_ID, registerWindow.get());
}

void setupChatWindow() {
    chatWindow = std::make_unique<gui::ChatWindow>();
    chatWindow->setOnInput([](auto inputText){
        auto separated = separateFirstWord(inputText);
        auto request = protocols::createPlayerCommandRequestMessage({separated.first, separated.second});
        networkingClient->send(protocols::serializeRequestMessage(request));
    });

    gameClient->addWindow(CHAT_WINDOW_ID, chatWindow.get());
}

void handleAuthResponse(const protocols::ResponseMessage& response) {
    auto authResponse = protocols::readAuthenticationResponseMessage(response);
    if(authResponse.success) {
        gameClient->switchToWindow(CHAT_WINDOW_ID);
        chatWindow->showText("Welcome!");
        return;
    }

    auto currentWindowName = gameClient->getCurrentWindowName();
    if (currentWindowName == LOGIN_WINDOW_ID) {
        loginWindow->showMessage(authResponse.msg);
    } else {
        registerWindow->showMessage(authResponse.msg);
    }
}

void handleDisplayResponse(const protocols::ResponseMessage& responseMessage) {
    auto displayMessage = protocols::readDisplayResponseMessage(responseMessage);
    chatWindow->showText(displayMessage);
}

void updateFromServer() {
    try {
        networkingClient->update();
    } catch (std::exception& e) {
        LOG(FATAL) << e.what();
    }

    auto incomingMessages = networkingClient->receive();
    if (incomingMessages.empty()) {
        return;
    }

    //convert text back to response messages
    std::vector<protocols::ResponseMessage> incomingResponses;
    incomingResponses.reserve(incomingMessages.size());
    std::transform(incomingMessages.begin(), incomingMessages.end(), std::back_inserter(incomingResponses),
                   [](auto& message) {
                       return protocols::deserializeResponseMessage(message);
                   });

    for(const auto& response : incomingResponses) {
        switch(response.header){
            case protocols::ResponseHeader::LOGIN_RESPONSE:
            case protocols::ResponseHeader::REGISTER_RESPONSE:
                handleAuthResponse(response);
                break;
            case protocols::ResponseHeader::DISPLAY_MESSAGE_RESPONSE:
                handleDisplayResponse(response);
                break;
            default:
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage:\n%s <ip address> <port>\ne.g. %s localhost 4002\n",
               argv[0], argv[0]);
        return 1;
    }

    FLAGS_log_dir = "./";
    google::InitGoogleLogging("GameClientLogs");

    gameClient = std::make_unique<gui::Application>();
    setupAuthWindow();
    setupLoginWindow();
    setupRegisterWindow();
    setupChatWindow();

    networkingClient = std::make_unique<Client>(argv[1], argv[2]);
    while(running) {
        gameClient->update();
        updateFromServer();
    }

    return 0;
}