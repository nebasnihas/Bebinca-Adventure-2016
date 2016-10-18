//
//  loginsystem.cpp
//  loginsystem
//

#include "Authenticator.hpp"
#include <fstream>



//Save file format(subject to change) remove comment after finalised


/*
 ------------------------------------------------------
 username.yml
 ------------------------------------------------------
 username:
 password:
 avatar type
 user level
 world
 */

protocols::LoginResponseCode Authenticator::login(const std::string& username, const std::string& password) {
    
    std::string file_user, file_pass; // vars to store file data
    std::string savefile_name = "savefiles/" + username + ".yml";
    
    if (username.length() > USERNAME_MAX_LENGTH) {
        return protocols::LoginResponseCode::INVALID_CREDENTIALS;
    }
    
    if(!save_file_exists(savefile_name)){
        //check if the save file exists
        return protocols::LoginResponseCode::USERNAME_NOT_FOUND;
    } else {
        
        //Load the yml file
        YAML::Node user_yaml = YAML::LoadFile(savefile_name);
        
        file_user = user_yaml["username"].as<std::string>();
        file_pass = user_yaml["password"].as<std::string>();
        
        //Check if credentials match
        if(file_user == username && file_pass == password){
            return protocols::LoginResponseCode::LOGIN_OK;
        } else {
            return protocols::LoginResponseCode::INVALID_CREDENTIALS;
        }
    }
}

protocols::RegistrationResponseCode Authenticator::registerAccount(const std::string& username, const std::string& password) {
    
    std::string savefile_name = "savefiles/" + username + ".yml";
    
    if (username.length() > USERNAME_MAX_LENGTH) {
        return protocols::RegistrationResponseCode::USERNAME_TOO_LONG;
    }
    
    if(save_file_exists(savefile_name)){
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
    
    std::string savefile_name = "savefiles/" + user + ".yml";
    
    
    //Create emitter with key:values
    YAML::Emitter credentials_map;
    credentials_map << YAML::BeginMap;
    credentials_map << YAML::Key << "username";
    credentials_map << YAML::Value << user;
    credentials_map << YAML::Key << "password";
    credentials_map << YAML::Value << pass;
    credentials_map << YAML::EndMap;
    
    
    std::ofstream f;
    f.open(savefile_name);
    f << credentials_map.c_str(); //Dump contents as string
    f.close();
}


