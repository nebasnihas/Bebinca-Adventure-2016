//
//  StrColorizer.hpp
//  parser
//
//
//

#ifndef StrColorizer_hpp
#define StrColorizer_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

//Standard ncurses colors
enum class color_type: char {
    
    BLACK   = 'b',
    RED     = 'r' ,
    GREEN   = 'g' ,
    YELLOW  = 'y',
    BLUE    = 'l',
    MAGENTA = 'm',
    CYAN    = 'c',
    WHITE   = 'w'
    
};
    
struct color_token {
    
    char character;
    color_type charcolor;
    //Each character has a character color attribute
    
};
    
typedef std::vector<color_token> attribute_string;

class StrColorizer {
    
private:
    static bool is_tag(char c);
    static color_type get_color(char c);
    static bool is_space(char c);
    
   friend std::ostream& operator<<(std::ostream& os, const attribute_string& att_str);
    
    
    
    //-------------Converter-----------------//
    static attribute_string str_to_colortoken (const std::string& text);
    
    //-------------Printer-------------------//
    static void colortoken_print (const attribute_string& att_str);
    
    
public:
    
    static void print_color(const std::string& text);
    
    
};
    

#endif /* parse_hpp */
