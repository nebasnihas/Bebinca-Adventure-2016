#ifndef ADVENTURE2016_MESSAGEBUILDER_HPP
#define ADVENTURE2016_MESSAGEBUILDER_HPP

#include "gsl/gsl"
#include "networking/server.h"
#include <vector>
#include <string>
#include <boost/optional.hpp>

struct MessageInfo {
    std::string message;
    networking::Connection client;
    boost::optional<std::string> sender;
};

class MessageBuilder {
public:
    virtual ~MessageBuilder() {}
    virtual std::vector<MessageInfo> buildMessages() const = 0;
protected:
    MessageBuilder(){};
};

#endif //ADVENTURE2016_MESSAGEBUILDER_HPP
