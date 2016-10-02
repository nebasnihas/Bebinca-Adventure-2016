#include "game/protocols/Authentication.hpp"
#include "boost/bimap.hpp"
#include "boost/bimap/unordered_set_of.hpp"
#include "boost/assign/list_of.hpp"
#include <vector>
#include "StringUtils.hpp"

using namespace boost::bimaps;

namespace protocols {
namespace {
typedef bimap<unordered_set_of<AuthType >, unordered_set_of<std::string>> mapType;

mapType map = boost::assign::list_of<mapType::relation>
        (AuthType::LOGIN, "login")
        (AuthType::REGISTER, "reg");

const std::string AUTH_OK = "ok";
const std::string AUTH_BAD = "bad";
const std::string AUTH_RESPONSE_BODY_SEPARATER = "/";
}

std::string createAuthRequestMessage(AuthType type, const std::string& username, const std::string& password)
{
    auto header = map.left.at(type);

    return header + " " + username + " " + password;
}
std::string createAuthResponseMessage(AuthType type, bool success, const std::string& messageBody)
{
    auto header = map.left.at(type);
    auto successValue = success? AUTH_OK : AUTH_BAD;

    return header + " " + successValue + " " + AUTH_RESPONSE_BODY_SEPARATER + messageBody;
}

AuthRequest readAuthRequestMessage(const std::string& text)
{
    auto separated = splitString(text);
    auto typeHeader = map.right.at(separated[0]);

    return AuthRequest{typeHeader, separated[1], separated[2]};
}
AuthResponse readAuthResponseMessage(const std::string& text)
{
    auto separated = splitString(text);
    auto typeHeader = map.right.at(separated[0]);
    auto success = separated[1] == AUTH_OK;
    auto body = text.substr(text.find(AUTH_RESPONSE_BODY_SEPARATER) + 1);

    return AuthResponse{typeHeader, success, body};
}

}