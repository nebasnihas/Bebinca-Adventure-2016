#ifndef ADVENTURE2016_PIGLATINDECORATOR_HPP
#define ADVENTURE2016_PIGLATINDECORATOR_HPP

#include "commands/MessageBuilder.hpp"

class PigLatinDecorator : public MessageBuilderDecorator {
public:
    PigLatinDecorator(const MessageBuilder& messageBuilder) : MessageBuilderDecorator{messageBuilder} {};
    virtual std::vector<MessageInfo> buildMessages() const override;
};


#endif //ADVENTURE2016_PIGLATINDECORATOR_HPP
