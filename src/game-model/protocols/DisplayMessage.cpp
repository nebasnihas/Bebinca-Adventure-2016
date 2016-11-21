#include "game/protocols/DisplayMessage.hpp"

namespace protocols
{

const std::string MESSAGE_KEY = "msg";
const std::string SENDER_KEY = "sender";

ResponseMessage createDisplayResponseMessage(const DisplayMessage& message)
{
    YAML::Node data;
    data[MESSAGE_KEY] = message.message;

    if (message.sender) {
        data[SENDER_KEY] = message.sender.get();
    }

    return ResponseMessage{ResponseHeader::DISPLAY_MESSAGE_RESPONSE, data};
}


std::string readDisplayResponseMessage(const ResponseMessage& responseMessage)
{
    std::string output;

    if (responseMessage.body[SENDER_KEY]) {
        auto sender = responseMessage.body[SENDER_KEY].as<std::string>();
        output += "[" + sender + "] - ";
    }

    output += responseMessage.body[MESSAGE_KEY].as<std::string>();

    return output;
}



}