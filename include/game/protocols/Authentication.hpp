#ifndef ADVENTURE2016_AUTH_H
#define ADVENTURE2016_AUTH_H

#include <string>

namespace protocols {
enum class AuthType {
    LOGIN,
    REGISTER,
};

struct AuthResponse {
    AuthType authType;
    bool success;
    std::string messageOrUserID;
};

struct AuthRequest {
    AuthType authType;
    std::string userName;
    std::string password;
};

//genric auth request for both login and registration
std::string createAuthRequestMessage(AuthType type, const std::string& username, const std::string& password);
std::string createAuthResponseMessage(AuthType type, bool sucess, const std::string& messageBody);

AuthRequest readAuthRequestMessage(const std::string& text);
AuthResponse readAuthResponseMessage(const std::string& text);
}

#endif //ADVENTURE2016_AUTH_H
