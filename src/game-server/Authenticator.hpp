
#ifndef loginsystem_hpp
#define loginsystem_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <boost/optional.hpp>
#include "yaml-cpp/yaml.h"

enum class LoginStatus {
    OK,
    USERNAME_NOT_FOUND,
    INVALID_CREDENTIALS,
};

enum class RegistrationStatus {
    OK,
    USERNAME_TOO_LONG,
    USERNAME_EXISTS
};

class Authenticator {
private:
    static void set_savefilevals(const std::string& user, const std::string& pass);
    static bool save_file_exists(const std::string& user);
    
public:
    static const int USERNAME_MAX_LENGTH = 10;
    
    static LoginStatus login(const std::string& username, const std::string& password);
    static RegistrationStatus registerAccount(const std::string& username, const std::string& password);
};

#endif /* loginsystem_hpp */
