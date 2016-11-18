/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "chatWindow.h"
#include "networking/client.h"
#include "game/protocols/RequestMessage.hpp"
#include "StringUtils.hpp"
#include "game/protocols/Authentication.hpp"
#include "game/protocols/PlayerCommand.hpp"
#include "game/protocols/DisplayMessage.hpp"
#include "MainMenuWindow.hpp"
#include "Application.hpp"
#include "LoginWindow.hpp"
#include <glog/logging.h>


using namespace networking;

protocols::UserCredentials extractCredentials(const std::string text)
{
    auto userAndPass = splitString(text);
    if (userAndPass.size() != 2) {
        throw std::exception{};
    }

    auto username = userAndPass[0];
    auto password = userAndPass[1];

    return protocols::UserCredentials{username, password};
}

enum class ClientState {
    INITIAL,
    LOGIN,
    REGISTER,
    IN_GAME,
};

static const std::string MAIN_MENU_WINDOW_ID = "auth";
static const std::string LOGIN_WINDOW_ID = "login";

ClientState currentState = ClientState::INITIAL;
bool running = true;

std::unique_ptr<Client> networkingClient;
std::unique_ptr<gui::Application> gameClient;
std::unique_ptr<gui::MainMenuWindow> authWindow;
std::unique_ptr<gui::LoginWindow> loginWindow;


void setupAuthWindow() {
    authWindow = std::make_unique<gui::MainMenuWindow>();
    authWindow->setOnSelection([](auto selection){
        switch(selection) {
            case gui::MainMenuChoice::LOGIN:
                currentState = ClientState::LOGIN;
                gameClient->switchToWindow(LOGIN_WINDOW_ID);
                break;
            case gui::MainMenuChoice::REGISTER:
                currentState = ClientState::REGISTER;
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
    loginWindow = std::make_unique<gui::LoginWindow>();
    loginWindow->setOnCancel([](){
        currentState = ClientState::INITIAL;
        gameClient->switchToWindow(MAIN_MENU_WINDOW_ID);
    });
    loginWindow->setOnInput([](auto user, auto password) {
        auto requestMessage = protocols::createLoginRequestMessage(protocols::UserCredentials{user, password});
        networkingClient->send(protocols::serializeRequestMessage(requestMessage));
    });

    gameClient->addWindow(LOGIN_WINDOW_ID, loginWindow.get());
}

void updateFromServer();
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

    networkingClient = std::make_unique<Client>(argv[1], argv[2]);
    while(running) {
        gameClient->update();
        updateFromServer();
    }

    return 0;
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

    switch (currentState) {
        case ClientState::REGISTER:
        case ClientState::LOGIN: {
            auto authResponseMessage = std::find_if(incomingResponses.begin(), incomingResponses.end(),
                                                    [](auto& msg) {
                                                        return msg.header ==
                                                               protocols::ResponseHeader::LOGIN_RESPONSE ||
                                                               msg.header ==
                                                               protocols::ResponseHeader::REGISTER_RESPONSE;
                                                    });

            if (authResponseMessage == incomingResponses.end()) {
                break;
            }

            auto loginResponse = protocols::readAuthenticationResponseMessage(*authResponseMessage);
//                chatWindow.displayText(loginResponse.msg);

            if (!loginResponse.success) {
                loginWindow->showMessage(loginResponse.msg);
                break;
            }

            currentState = ClientState::IN_GAME; //TODO go to character select
            return; //TODO SWITCH WINDOW
            break;
        }
        case ClientState::IN_GAME: {
            for (const auto& responseMessage : incomingResponses) {
                if (responseMessage.header == protocols::ResponseHeader::DISPLAY_MESSAGE_RESPONSE) {
                    auto displayMessage = protocols::readDisplayResponseMessage(responseMessage);
//                        chatWindow.displayText(displayMessage);
                }
            }
            break;
        }
        default:
            break;
    }
}

