/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "chatWindow.h"
#include "networking/client.h"
#include "game/protocols/Message.hpp"

#include <string>
#include "StringUtils.hpp"
#include "game/protocols/Authentication.hpp"


using namespace networking;

//TODO this is placeholder authentication window
bool authenticated = false;

void promptForAuth(ChatWindow& chatWindow)
{
    chatWindow.displayText("Enter your username and password seperated by space\n");
    chatWindow.update();
}

void checkForAuthMessage(const std::string &message, ChatWindow &chatWindow)
{
    auto response = protocols::readAuthResponseMessage(message);
    if (response.success) {
        authenticated = true;
        chatWindow.displayText("You have logged in as " + response.messageOrUserID + "\n");
    } else {
        chatWindow.displayText(response.messageOrUserID);
        chatWindow.update();
    }
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage:\n%s <ip address> <port>\ne.g. %s localhost 4002\n",
               argv[0], argv[0]);
        return 1;
    }

    Client client{argv[1], argv[2]};

    bool done = false;
    auto onTextEntry = [&done, &client](std::string text) {
        if ("exit" == text || "quit" == text) {
            done = true;
        } else if (!authenticated) {
            auto userAndPass = splitString(text);
            if (userAndPass.size() == 2) {
                auto request = protocols::createAuthRequestMessage(protocols::AuthType::LOGIN, userAndPass[0], userAndPass[1]);
                client.send(protocols::createMessage(protocols::MessageType::AUTHENTICATION, request));
            }
        } else {
            client.send(protocols::createMessage(protocols::MessageType::USER_COMMAND, text));
        }
    };

    ChatWindow chatWindow(onTextEntry);
    promptForAuth(chatWindow);

    while (!done && !client.isDisconnected()) {
        try {
            client.update();
        } catch (std::exception &e) {
            chatWindow.displayText("Exception from Client update:");
            chatWindow.displayText(e.what());
            done = true;
        }

        auto response = client.receive();
        if (!response.empty()) {
            for (const auto& text : response) {
                auto message = protocols::readMessage(text);

                if (message.type == protocols::MessageType::CLIENT_DISPLAY_MESSAGE) {
                    chatWindow.displayText(message.messageBody);
                } else if (message.type == protocols::MessageType::AUTHENTICATION) {
                    if (!authenticated) {
                        checkForAuthMessage(message.messageBody, chatWindow);
                    }
                }
            }
        }
        chatWindow.update();
    }

    return 0;
}

