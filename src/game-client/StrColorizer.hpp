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
#include <ncurses.h>

//Standard ncurses colors
enum class color_type: char {

    BLACK   = 'l',
    RED     = 'r' ,
    GREEN   = 'g' ,
    YELLOW  = 'y',
    BLUE    = 'b',
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
    static void colortoken_print (WINDOW *win, int ypos, int xpos,const attribute_string& att_str);


public:

    static void print_color(WINDOW *win, int ypos, int xpos, const std::string& text);


};


#endif /* parse_hpp */
