#ifndef ADVENTURE2016_MESSAGEBUILDER_HPP
#define ADVENTURE2016_MESSAGEBUILDER_HPP

#include "gsl/gsl"
#include "networking/server.h"
#include <vector>

class MessageBuilder {
public:
    virtual ~MessageBuilder() {}
    virtual std::vector<networking::Message> buildMessages() const = 0;
protected:
    MessageBuilder(){};
};

#endif //ADVENTURE2016_MESSAGEBUILDER_HPP
