//
//  loginsystem.cpp
//  loginsystem
//

#include "Authentication.hpp"
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

std::string login::welcomescreen(){
    int choice;
    
    for (int i=0; i < 50; i++) {
        std::cout << "-";
    }
    
    std::cout << std::endl << " Welcome to Bebincaventure" <<std::endl;
    
    for (int i=0; i < 50; i++) {
        std::cout << "-";
    }
    
    std::cout << std::endl
    << std::endl
    << "Select an option" <<std::endl;
    
    std::cout << "1. Login" << std::endl
    << "2. Register\n";
    
    std::cin >> choice;
    
    switch (choice) {
        case 1:
            username = _login();
            break;
        case 2:
            username = _register();
            break;
            
        default:
            std::cout << "Invalid choice\n";
            username= "nousercreated";
            break;
    }
    
    return username;
}

std::string login::_login(){
    std::string user, pass; // vars to store user input
    std::string file_user, file_pass; // vars to store file data
    bool flag = false;
    
    while (flag == false){
        std::cout << "Enter credentials" <<std::endl
        << "Please enter username\n";
        std::cin >> user;
        
        assert(user.size()<=10 && "The username cannot contain more than 10 characters"); //For testing
        
        std::cout << "Please enter password\n";
        std::cin >> pass;
        
        std::cout << save_file_exists(user +".txt"); //Remove this (for testing)
        
        if(!save_file_exists(user + ".txt")){
            //check if the save file exists
            std::cout << "Username has not been registererd\n";
            flag = false;
        } else {
            //Check if credentials match
            std::ifstream f(user + ".txt");
            getline(f, file_user);
            getline(f, file_pass);
            
            if(file_user == user && file_pass == pass){
                flag = true;
                std::cout << "Login successful\n";
                break;
            } else {
                std::cout << "The credentials do not match\n";
                flag = false;
            }
            
        }
        
    }
    
    return user;
}

std::string login::_register(){
    std::string user, pass; // vars to store user input
    std::cout << "Create credentials" <<std::endl;
    bool flag = false;
    
    
    while (flag == false){
        
        std::cout << "Please select username\n";
        std::cin >> user;
        
        assert(user.size()<=10 && "The username cannot contain more than 10 characters"); //For testing
        
        if(save_file_exists(user +".txt")){
            std::cout << "Username is already taken\n";
            flag = false;
            
        } else {
            flag = true;
            
        }
        
    }
    std::cout << "Please input password\n";
    std::cin >> pass;
    
    set_savefilevals(user, pass);
    
    return user;
}

bool login::save_file_exists(const std::string user){
    std::ifstream f(user);
    return f.good();
}

void login::set_savefilevals(const std::string user, std::string pass){
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

