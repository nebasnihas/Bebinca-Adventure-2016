#include "game/protocols/DisplayMessage.hpp"
#include "game/protocols/RequestMessage.hpp"
#include "game/protocols/ResponseMessage.hpp"

namespace protocols
{

const std::string MESSAGE_KEY = "msg";

ResponseMessage createDisplayResponseMessage(const DisplayMessage& message)
{
    YAML::Node data;
    data[MESSAGE_KEY] = message.message;

    return ResponseMessage{ResponseHeader::DISPLAY_MESSAGE_RESPONSE, data};
}


DisplayMessage readDisplayResponseMessage(const ResponseMessage& responseMessage)
{
    auto messageToDisplay = responseMessage.body[MESSAGE_KEY].as<std::string>();

    return DisplayMessage{messageToDisplay};
}

}