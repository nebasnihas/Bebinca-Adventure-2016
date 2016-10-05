#ifndef ADVENTURE2016_DISPLAYMESSAGE_HPP
#define ADVENTURE2016_DISPLAYMESSAGE_HPP

#include <string>
#include "RequestMessage.hpp"
#include "ResponseMessage.hpp"
#include "yaml-cpp/yaml.h"

namespace protocols {
//for now just a string, later on there will  be other stuff such as formatting and stuff
struct DisplayMessage {
    std::string message;
};

ResponseMessage createDisplayResponseMessage(const DisplayMessage& message);
DisplayMessage readDisplayResponseMessage(const ResponseMessage& responseMessage);


}

#endif //ADVENTURE2016_DISPLAYMESSAGE_HPP
