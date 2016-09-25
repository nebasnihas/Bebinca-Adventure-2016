
#include <iostream>
#include <string>
#include <fstream>

//Global Vars
std::string username;

//Save file format
/*
 ------------------------------------------------------
 username.txt
 ------------------------------------------------------
 username
 password
 avatar type
 user level
 
 */


void welcomescreen(); //Call this function only
std::string login();
std::string _register();
/*Returns the username for successful login/registration. Other methods can use the username to open game save file and modify/access it.*/
bool save_file_exists (std::string user);

int main(int argc, const char * argv[]) {
    
    welcomescreen();
    return 1;
}



void welcomescreen(){
    
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
            username = login();
            break;
        case 2:
            username = _register();
            break;
            
        default:
            std::cout << "Invalid choice\n";
            break;
    }
    
    //Break out to character creation from here
    
}

std::string login(){
    std::string user, pass; // vars to store user input
    std::string file_user, file_pass; // vars to store file data
    bool flag = false;
    
    while (flag == false){
    std::cout << "Enter credentials" <<std::endl
              << "Please enter username\n";
    std::cin >> user;
    
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

//Must refine function _register. Has issues with file checking
std::string _register(){
    std::string user, pass; // vars to store user input
    std::cout << "Create credentials" <<std::endl;
    bool flag = false;

    
    while (flag == false){

        std::cout << "Please select username\n";
        std::cin >> user;
        
        if(save_file_exists(user)){
            std::cout << "Username is already taken\n";
            flag = false;
        } else {
            flag = true;
            break;
        }
        
    }
    std::cout << "Please input password\n";
    std::cin >> pass;
    
    std::ofstream f;
    f.open(user + ".txt");
    f << user
      << std::endl
      << pass
      << std::endl;
    
    f.close();
    return user;
}

bool save_file_exists (std::string user){
    std::ifstream f(user);
    return f.good();
}