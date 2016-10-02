#ifndef ADVENTURE2016_MESSAGE_H
#define ADVENTURE2016_MESSAGE_H

#include <string>

namespace protocols {

enum class MessageType {
    SERVER_COMMAND,
    USER_COMMAND,
    AUTHENTICATION,
    CLIENT_DISPLAY_MESSAGE,
};


struct MessageData {
    MessageType type;
    std::string messageBody;
};


std::string createMessage(MessageType messageType, const std::string& text);
MessageData readMessage(const std::string& message);



}

#endif //ADVENTURE2016_MESSAGE_H
