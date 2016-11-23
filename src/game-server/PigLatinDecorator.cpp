#include <game/protocols/ResponseMessage.hpp>
#include <game/protocols/DisplayMessage.hpp>
#include "PigLatinDecorator.hpp"

std::vector<MessageInfo> PigLatinDecorator::buildMessages() const {
    auto messages = MessageBuilderDecorator::buildMessages();
    for(auto& msg : messages) {
        //TODO check character status
        protocols::ResponseMessage responseMessage = msg.message;
        switch (responseMessage.header) {
            case protocols::ResponseHeader::DISPLAY_MESSAGE_RESPONSE: {
                auto displayMsg = protocols::readDisplayResponseMessage(responseMessage);
                displayMsg.message += " --TODO:pig latin";

                auto editedResponse = protocols::createDisplayResponseMessage(displayMsg);
                msg.message = editedResponse;
                break;
            }
            default:
                break;
        }
    }

    return messages;
}
