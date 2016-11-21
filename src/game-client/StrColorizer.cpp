//
//  StrColorizer
//  parser
//
//

#include "StrColorizer.hpp"


typedef std::vector<color_token> attribute_string;

bool StrColorizer::is_tag(char c){
    return c == '&';
}

bool StrColorizer::is_space(char c){
    return c == ' '  ||
    c == '\n' ||
    c == '\r' ||
    c == '\t';
}


void StrColorizer::print_color(const std::string &text){
    
    attribute_string res = str_to_colortoken(text);
    
    for (color_token ct: res) {
        
        if (is_space(ct.character)){
            std::cout << "The char is a space "
            << " Color: " << char(ct.charcolor)
            << std::endl;
        } else {
        
        std::cout << "The char: " << ct.character
                  << " Color: " << char(ct.charcolor)
                  << std::endl;
        }
    }
    
    
}

/*
std::ostream& operator<<(std::ostream& os, const color_token& ct){
    
    
    for (color_token ct: res) {
        
        if (is_space(ct.character)){
            os << "The char is a space "
            << " Color: " << char(ct.charcolor)
            << std::endl;
        } else {
            
            os << "The char: " << ct.character
            << " Color: " << char(ct.charcolor)
            << std::endl;
        }
    }

    
    
    return os;
}
*/

color_type StrColorizer::get_color(char c){
    
    switch (c) {
        case 'b':
            return color_type::BLACK;
            break;
            
        case 'r':
            return color_type::RED;
            break;
            
        case 'g':
            return color_type::GREEN;
            break;
            
        case 'y':
            return color_type::YELLOW;
            break;
            
        case 'l':
            return color_type::BLUE;
            break;
            
        case 'm':
            return color_type::MAGENTA;
            break;
            
        case 'c':
            return color_type::CYAN;
            break;
            
        default:
            return color_type::WHITE;
            break;
    }
    
}

attribute_string StrColorizer::str_to_colortoken(const std::string &text){
    
    attribute_string att_str_vector;
    
    for (int i=0; i < text.size(); i++) {
        
        if(!is_tag(text[i])){
          att_str_vector.push_back(color_token{text[i], color_type::WHITE});
        }else {
          
          i++;
          color_type c_type = get_color(text[i]);
          i++;
          
          while (!is_tag(text[i]) && i< text.size()){
              
              att_str_vector.push_back(color_token { text[i], c_type});
              i++;
                
          }
            

        }
        
    }
        
    
    
    return att_str_vector;
}

