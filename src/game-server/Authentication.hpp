
#ifndef loginsystem_hpp
#define loginsystem_hpp

#include <stdio.h>
#include <iostream>
#include <string>


class login{
private:
    std::string username, password;
    void set_savefilevals(const std::string user, std::string pass);
    bool save_file_exists(const std::string user);
    std::string _login();
    std::string _register();
    
public:
    login();
    std::string welcomescreen();
    /*Call this function only. Returns the string:username after savefile is created. 
     Returns "nousercreated" if the user decides to quit before creating or logging in*/

    
};

#endif /* loginsystem_hpp */
