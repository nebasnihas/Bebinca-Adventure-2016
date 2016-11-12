#include "PigLatinDecorator.hpp"

std::vector<MessageInfo> PigLatinDecorator::buildMessages() const {
    auto messages = MessageBuilderDecorator::buildMessages();
    for(auto& msg : messages) {
        //TODO check character status
        msg.message += " --TODO:pig latin";
    }

    return messages;
}
