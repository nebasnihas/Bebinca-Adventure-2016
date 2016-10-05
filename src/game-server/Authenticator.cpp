//
//  loginsystem.cpp
//  loginsystem
//

#include "Authenticator.hpp"
#include <fstream>
#include <assert.h>

//Save file format(subject to change)
/*
 ------------------------------------------------------
 username.txt
 ------------------------------------------------------
 username
 password
 avatar type
 user level
 world
 */

protocols::LoginResponseCode Authenticator::login(const std::string& username, const std::string& password) {
    std::string file_user, file_pass; // vars to store file data

    if (username.length() > USERNAME_MAX_LENGTH) {
        return protocols::LoginResponseCode::INVALID_CREDENTIALS;
    }

    if(!save_file_exists(username + ".txt")){
        //check if the save file exists
        return protocols::LoginResponseCode::USERNAME_NOT_FOUND;
    } else {
        //Check if credentials match
        std::ifstream f(username + ".txt");
        getline(f, file_user);
        getline(f, file_pass);

        if(file_user == username && file_pass == password){
            return protocols::LoginResponseCode::LOGIN_OK;
        } else {
            return protocols::LoginResponseCode::INVALID_CREDENTIALS;
        }
    }
}

protocols::RegistrationResponseCode Authenticator::registerAccount(const std::string& username, const std::string& password) {
    std::cout << "Create credentials" <<std::endl;

    if (username.length() > USERNAME_MAX_LENGTH) {
        return protocols::RegistrationResponseCode::USERNAME_TOO_LONG;
    }

    if(save_file_exists(username +".txt")){
        return protocols::RegistrationResponseCode::USERNAME_EXISTS;
    }

    set_savefilevals(username, password);

    return protocols::RegistrationResponseCode::REGISTRATION_OK;
}

bool Authenticator::save_file_exists(const std::string& user){
    std::ifstream f(user);
    return f.good();
}

void Authenticator::set_savefilevals(const std::string& user, const std::string& pass){
    int user_level = 0;
    std::string avatar = "nil";
    std::string world = "start";
    
    std::ofstream f;
    f.open(user + ".txt");
    f << user << std::endl
    << pass  << std::endl
    << avatar << std::endl
    << user_level << std::endl
    << world << std::endl;
    
    f.close();
}


