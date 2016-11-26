/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include <glog/logging.h>
#include "networking/client.h"
#include "StringUtils.hpp"
#include "game/protocols/RequestMessage.hpp"
#include "game/protocols/Authentication.hpp"
#include "game/protocols/PlayerCommand.hpp"
#include "game/protocols/DisplayMessage.hpp"
#include "game/protocols/CommandInfo.hpp"
#include "game/protocols/Editing.hpp"
#include "game/GameDataImporter.hpp"
#include "Application.hpp"
#include "MainMenuWindow.hpp"
#include "AuthenticationWindow.hpp"
#include "ChatWindow.hpp"
#include "WorldBuildingWindow.hpp"

using namespace networking;

static const std::string MAIN_MENU_WINDOW_ID = "auth";
static const std::string LOGIN_WINDOW_ID = "login";
static const std::string REGISTER_WINDOW_ID = "register";
static const std::string CHAT_WINDOW_ID = "chat";
static const std::string WORLDBUILDING_WINDOW_ID = "build";
bool running = true;

std::unique_ptr<Client> networkingClient;
std::unique_ptr<gui::Application> gameClient;
std::unique_ptr<gui::MainMenuWindow> authWindow;
std::unique_ptr<gui::AuthenticationWindow> loginWindow;
std::unique_ptr<gui::AuthenticationWindow> registerWindow;
std::unique_ptr<gui::ChatWindow> chatWindow;
std::unique_ptr<gui::WorldBuildingWindow> worldBuildingWindow;

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

    chatWindow->setOnSoftKeyPressed([](auto key) {
        switch(key) {
            case gui::SoftKey::F1: {
                break;
            }
            default:
                break;
        }
    });

    gameClient->addWindow(CHAT_WINDOW_ID, chatWindow.get());
}

void setupWorldBuildingWindow() {
    worldBuildingWindow = std::make_unique<gui::WorldBuildingWindow>();

    worldBuildingWindow->setOnSubmit([](auto area) {
        YAML::Emitter em;
        em << YAML::Node{area};
        std::stringstream arguments;
        arguments << "submit '" << em.c_str() << "'";
        auto args = arguments.str();
        auto cmd =  protocols::createPlayerCommandRequestMessage(protocols::PlayerCommand{command : "edit", arguments : args});
        auto request = protocols::serializeRequestMessage(cmd);
        networkingClient->send(request);

        worldBuildingWindow->showMessage("Saving changes..");
    });

    worldBuildingWindow->setOnQuit([]() {
       gameClient->switchToWindow(CHAT_WINDOW_ID);
    });

    worldBuildingWindow->setOnDiscard([]() {

    });

    worldBuildingWindow->setOnQuit([]() {
        gameClient->switchToWindow(CHAT_WINDOW_ID);
    });

    gameClient->addWindow(WORLDBUILDING_WINDOW_ID, worldBuildingWindow.get());
}

void handleAuthResponse(const protocols::ResponseMessage& response) {
    auto authResponse = protocols::readAuthenticationResponseMessage(response);
    if(authResponse.success) {
        gameClient->switchToWindow(CHAT_WINDOW_ID);
        chatWindow->showText("Welcome!");
        auto cmd = protocols::createCommandInfoRequest(protocols::CommandName::EDIT);
        networkingClient->send(protocols::serializeRequestMessage(cmd));
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

    std::string displayText;
    if (displayMessage.sender) {
        displayText += "[" + displayMessage.sender.get() + "] - ";
    }
    displayText += displayMessage.message;

    chatWindow->showText(displayText);
}

void handleCommandResponse(const protocols::ResponseMessage& responseMessage) {
    auto response = protocols::readCommandInfoResponse(responseMessage);
    switch(response.name) {
        case protocols::CommandName::EDIT:
            chatWindow->showText(response.inputBinding);
            break;
        default:
            break;
    }
}

void handleEditResponse(const protocols::ResponseMessage& responseMessage) {
    auto editResponse = protocols::readEditResponse(responseMessage);
    switch (editResponse.editType) {
        case protocols::EditType::AREA: {
            CHECK(editResponse.data) << "No data for area";
            auto areaNode = editResponse.data.get();
            auto area = GameDataImporter::getRooms(areaNode);

            if (editResponse.success) {
                //Succesfully requested area to edit
                worldBuildingWindow->loadAreaData(area[0]);
            } else if (!(editResponse.success || worldBuildingWindow->hasAreaData())) {
                //This means player was disconnected while editing. Resume from where left off
                worldBuildingWindow->loadAreaData(area[0]);
            }

            gameClient->switchToWindow(WORLDBUILDING_WINDOW_ID);
            break;
        }
        default:
            break;
    }
}

void updateFromServer() {
    if (networkingClient->isDisconnected()) {
        running = false;
        return;
    }

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
            case protocols::ResponseHeader::COMMAND_INFO_RESPONSE:
                handleCommandResponse(response);
                break;
            case protocols::ResponseHeader::EDIT_INFO_RESPONSE:
                handleEditResponse(response);
                break;
            default:
                break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage:\n%s <ip address> <port>\ne.g. %s localhost 4000\n",
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
    setupWorldBuildingWindow();

    networkingClient = std::make_unique<Client>(argv[1], argv[2]);
    while(running) {
        gameClient->update();
        updateFromServer();
    }

    return 0;
}