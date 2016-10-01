/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "chatWindow.h"
#include "networking/client.h"

#include <string>

using namespace networking;

//TODO this is placeholder authentication
bool authenticated = false;

void promptForAuth(ChatWindow& chatWindow)
{
    chatWindow.displayText("Enter your username and password seperated by space\n");
    chatWindow.update();
}

void checkForAuthMessage(const std::string &message, ChatWindow &chatWindow)
{
    if (message == "auth:ok") {
        authenticated = true;
        chatWindow.displayText("You have logged in\n");
    } else if (message == "auth:bad") {
        promptForAuth(chatWindow);
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
            client.send("auth " + text);
        } else {
            client.send(text);
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
            for (const auto& message : response) {
                if (!authenticated) {
                    checkForAuthMessage(message, chatWindow);
                } else {
                    chatWindow.displayText(message);
                }
            }
        }
        chatWindow.update();
    }

    return 0;
}

