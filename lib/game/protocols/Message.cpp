#include "game/protocols/Message.hpp"
#include "boost/bimap/bimap.hpp"
#include "boost/bimap/unordered_set_of.hpp"
#include "boost/assign/list_of.hpp"
#include "StringUtils.hpp"

using namespace boost::bimaps;

namespace protocols {
namespace {
typedef bimap<unordered_set_of<MessageType>, unordered_set_of<std::string>> mapType;

mapType headerMap = boost::assign::list_of<mapType::relation>
        (MessageType::SERVER_COMMAND, "scmd")
        (MessageType::USER_COMMAND, "ucmd")
        (MessageType::AUTHENTICATION, "auth")
        (MessageType::CLIENT_DISPLAY_MESSAGE, "cdis");
}

std::string createMessage(MessageType messageType, const std::string &text) {
    auto header = headerMap.left.at(messageType);

    return header + " " + text;
}

MessageData readMessage(const std::string &message) {
    auto extracted = separateFirstWord(message);

    auto header = headerMap.right.at(extracted.first);
    return MessageData{header, extracted.second};
}

}