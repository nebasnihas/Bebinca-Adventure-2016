#ifndef ADVENTURE2016_MESSAGEIO_HPP
#define ADVENTURE2016_MESSAGEIO_HPP

class MessageBuilder;

class MessageIO {
public:
    virtual void send(const MessageBuilder& messageBuilder) = 0;
};

#endif //ADVENTURE2016_MESSAGEIO_HPP
