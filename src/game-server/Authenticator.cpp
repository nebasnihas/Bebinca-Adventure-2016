//
//  loginsystem.cpp
//  loginsystem
//

#include "Authenticator.hpp"
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
//#include <boost/filesystem.hpp> does not compile



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

std::string Authenticator::get_saveloc(const std::string& user){
    std::string send_loc = "savefiles/" + user + ".yml";
    return send_loc;
}

protocols::LoginResponseCode Authenticator::login(const std::string& username, const std::string& password) {
    
    std::string file_user, file_pass; // vars to store file data
    std::string savefile_name = get_saveloc(username);
    
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
    
    std::string savefile_name = get_saveloc(username);
    
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

    /*
    const char save_path[] = "savefile/";

    boost::filesystem::path loc (save_path);

    if(!boost::filesystem::exists(loc)) {
        boost::filesystem::create_directory(loc);
    }

    */

    //Check for directory if does not exist then create it.
    struct stat stct = {0};
    if(stat("savefiles/", &stct) == -1){
        mkdir("savefiles/",0700);
    }


        std::string savefile_name = get_saveloc(user);

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


