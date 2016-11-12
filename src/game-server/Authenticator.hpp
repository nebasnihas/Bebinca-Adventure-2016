
#ifndef loginsystem_hpp
#define loginsystem_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <boost/optional.hpp>
#include "yaml-cpp/yaml.h"
#include "game/protocols/Authentication.hpp"

enum class PlayerRole {
    NORMAL = 1 << 0,
    ADMIN = 1 << 1,
    WORLDBUILDER = 1 << 2,
};

struct AccountInfo {
    std::string username;
    int playerRoles;

    bool operator==(const AccountInfo& other) const {
        return username == other.username;
    }
};

struct AccountInfoHash {
    size_t operator()(const AccountInfo& other) const
    {
        return std::hash<std::string>()(other.username);
    }
};

template <class T>
struct AuthResult {
    T result;
    boost::optional<AccountInfo> account;
};

class Authenticator {
private:
    static void set_savefilevals(const std::string& user, const std::string& pass);
    static bool save_file_exists(const std::string& user);
    static std::string get_saveloc(const std::string& user);
    
public:
    static const int USERNAME_MAX_LENGTH = 10;
    
    static AuthResult<protocols::LoginResponseCode> login(const std::string& username, const std::string& password);
    static AuthResult<protocols::RegistrationResponseCode> registerAccount(const std::string& username,
                                                                           const std::string& password);
};

#endif /* loginsystem_hpp */
