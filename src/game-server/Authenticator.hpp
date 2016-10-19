
#ifndef loginsystem_hpp
#define loginsystem_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <boost/optional.hpp>
#include "yaml-cpp/yaml.h"
#include "game/protocols/Authentication.hpp"



class Authenticator {
private:
    static void set_savefilevals(const std::string& user, const std::string& pass);
    static bool save_file_exists(const std::string& user);
    static std::string get_saveloc(const std::string& user);
    
public:
    static const int USERNAME_MAX_LENGTH = 10;
    
    static protocols::LoginResponseCode login(const std::string& username, const std::string& password);
    static protocols::RegistrationResponseCode registerAccount(const std::string& username, const std::string& password);
};

#endif /* loginsystem_hpp */
