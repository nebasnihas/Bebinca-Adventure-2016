/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "chatWindow.h"
#include "networking/client.h"
#include "game/protocols/RequestMessage.hpp"

#include <string>
#include "StringUtils.hpp"
#include "game/protocols/Authentication.hpp"
#include <algorithm>
#include "game/protocols/PlayerCommand.hpp"
#include "game/protocols/DisplayMessage.hpp"


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

ClientState currentState = ClientState::INITIAL;
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage:\n%s <ip address> <port>\ne.g. %s localhost 4002\n",
               argv[0], argv[0]);
        return 1;
    }

    Client client{argv[1], argv[2]};
    ChatWindow chatWindow{[](std::string){}};

    //TODO placeholder states, transitions, text
    bool done = false;
    auto onTextEntry = [&done, &client, &chatWindow](std::string text) {
        if ("exit" == text || "quit" == text) {
            done = true;
        }

        switch(currentState) {
            case ClientState::INITIAL:
                //TODO separate inputs for username and password. use space separation for now
                if (text == "register") {
                    currentState = ClientState::REGISTER;
                    chatWindow.displayText("Enter the username you want and your password separated by space. Type cancel to go back"); //TODO ask to confirm password
                } else if (text == "login") {
                    chatWindow.displayText("Enter you username and password separated by a space. Type cancel go back");
                    currentState = ClientState::LOGIN;
                } else {
                    chatWindow.displayText("type 'login' or 'register'");
                }
                break;
            case ClientState::LOGIN:
                if (text == "cancel") {
                    currentState = ClientState::INITIAL;
                    chatWindow.displayText("Type register or login to begin");
                    break;
                }

                try {
                    auto creds = extractCredentials(text);
                    auto requestMessage = protocols::createLoginRequestMessage(creds);
                    client.send(protocols::serializeRequestMessage(requestMessage));
                } catch (const std::exception&) {
                    chatWindow.displayText("Can you read the instructions");
                }
                break;
            case ClientState::REGISTER:
                if (text == "cancel") {
                    currentState = ClientState::INITIAL;
                    chatWindow.displayText("Type register or login to begin");
                    break;
                }

                try {
                    auto creds = extractCredentials(text);
                    auto requestMessage = protocols::createRegistrationRequestMessage(creds);
                    client.send(protocols::serializeRequestMessage(requestMessage));
                } catch (const std::exception&) {
                    chatWindow.displayText("NOPE\n");
                }
                break;
            case ClientState::IN_GAME:
                auto separated = separateFirstWord(text);
                protocols::PlayerCommand command{separated.first, separated.second};

                auto requestMessage = protocols::createPlayerCommandRequestMessage(command);
                client.send(protocols::serializeRequestMessage(requestMessage));
        }

        chatWindow.update();
    };


    chatWindow.displayText("Type register or login to begin");
    chatWindow.setOnTextEntry(onTextEntry);
    chatWindow.update();

    while (!done && !client.isDisconnected()) {
        chatWindow.update();
        try {
            client.update();
        } catch (std::exception &e) {
            chatWindow.displayText("Exception from Client update:");
            chatWindow.displayText(e.what());
            done = true;
        }

        auto incomingMessages = client.receive();
        if (incomingMessages.empty()) {
            continue;
        }

        //convert text back to response messages
        std::vector<protocols::ResponseMessage> incomingResponses;
        incomingResponses.reserve(incomingMessages.size());
        std::transform(incomingMessages.begin(), incomingMessages.end(), std::back_inserter(incomingResponses), [](auto& message) {
            return protocols::deserializeResponseMessage(message);
        });

        switch (currentState) {
            case ClientState::REGISTER:
            case ClientState::LOGIN: {
                auto authResponseMessage = std::find_if(incomingResponses.begin(), incomingResponses.end(),
                                                         [](auto& msg) {
                                                             return msg.header == protocols::ResponseHeader::LOGIN_RESPONSE ||
                                                                    msg.header == protocols::ResponseHeader::REGISTER_RESPONSE;
                                                         });

                if (authResponseMessage == incomingResponses.end()) {
                    break;
                }

                auto loginResponse = protocols::readAuthenticationResponseMessage(*authResponseMessage);
                chatWindow.displayText(loginResponse.msg);

                if (loginResponse.success) {
                    currentState = ClientState::IN_GAME; //TODO go to character select
                }
                break;
            }
            case ClientState::IN_GAME: {
                for (const auto& responseMessage : incomingResponses) {
                    if (responseMessage.header == protocols::ResponseHeader::DISPLAY_MESSAGE_RESPONSE) {
                        auto displayMessage = protocols::readDisplayResponseMessage(responseMessage);
                        chatWindow.displayText(displayMessage);
                    }
                }
                break;
            }
            default:
                break;
        }
    }

    return 0;
}

