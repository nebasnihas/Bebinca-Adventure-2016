//AsciiPrinter

#include "AsciiPrinter.hpp"

#define MAX_SHADES 10

typedef unsigned short uint16;
typedef unsigned int uint32;

using namespace std;

uint16 AsciiPrinter::extractShort (ifstream &f)
{
    char buf[2];
    f.read (buf, 2);
    uint16 value = buf[0] | (buf[1] << 8);
    return value;
}

uint32 AsciiPrinter::extractInt (ifstream &f)
{
    char buf[4];
    f.read (buf, 4);
    uint32 value = buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
    return value;
}

void AsciiPrinter::to_lower(string& word){
    
    transform(word.begin(), word.end(), word.begin(), ::tolower);
}

string AsciiPrinter::get_loc(const string &font_type){
    
    string type = font_type;
    to_lower(type);
    
    const string find_loc = "assets/fontsets/" + type + "/";
    return find_loc;
}

void AsciiPrinter::process_letter_location(const char &c, string &word_file){
    
    word_file += c;
    word_file += ".txt";
    
}

int AsciiPrinter::get_word_size(string &word){
    long long_word = word.size();
    const int word_size = int(long_word);
    return word_size;
}

string AsciiPrinter::process_font_type(const string &font_type){
    
    string type = font_type;
    to_lower(type);
    
    if(type == "default"){
        return "default";
    } else if (type == "test"){
        return "test";
    }//Add more font types here
    
    return "default";
}

void AsciiPrinter::readHeader(ifstream &f, BMPFileHeader &header){
    
    header.filetype = extractShort (f);
    header.filesize = extractInt (f);
    header.reserved = extractInt (f);
    header.offset = extractInt (f);
    header.bytesInHeader = extractInt (f);
    header.width = extractInt (f);
    header.height = extractInt (f);
    header.planes = extractShort (f);
    header.bitsPerPixel = extractShort (f);
    header.compression = extractInt (f);
    header.size = extractInt (f);
    header.horizRes = extractInt (f);
    header.vertRes = extractInt (f);
    header.indicesUsed = extractInt (f);
    header.indicesImportant = extractInt (f);
}

string AsciiPrinter::printObject1(const string style, const string objname){
    return printObject0(style,objname);
}

string AsciiPrinter::printObject0(const string style, const string &objname){
    
    int width, height;
    unsigned char *image;
    char def_shades[MAX_SHADES] = {'#','$','O','=','+','|','-','^','.',' '};
    char num_shades[MAX_SHADES] = {'0','1','2','3','4','5','6','7','8','.'};
    
    // = {'#','$','O','=','+','|','-','^','.',' '};
    // = {'0','1','2','3','4','5','6','7','8','9'};
    int average_color = 0;
    
    
    string ret_string;
    ifstream bmpfile;
    BMPFileHeader header;
    
    const string filepath = "assets/bmpimgs/" + objname + ".bmp";
    
    // Open the image file
    bmpfile.open (filepath, ios::in | ios::binary);
    if ( ! bmpfile ) {
        //cout << "cannot open bmp file" << endl;
        return "File not found";
    }
    
    // Read header
    readHeader (bmpfile, header);
    
    // Read image
    width = int(header.width);
    if ( width < 0 )
        width *= -1;
    height = int(header.height);
    if ( height < 0 )
        height *= -1;
    
    int rowsize = width * 3;
    
    image = new unsigned char [ rowsize * height ];
    
    bmpfile.seekg ( header.offset, ios::beg );
    bmpfile.read ( (char *)image, 3*width*height );
    bmpfile.close();
    
    //char tempoutput[3][3];
    //string temo;
    for(int y = height-1; y >= 0; y--) {
        
        for(int x = 0; x < width; x++) {
            
            // Get the average color
            average_color = ( image[x*3     + y*rowsize] +
                             image[x*3 + 1 + y*rowsize] +
                             image[x*3 + 2 + y*rowsize] ) / 3;
            
            // Convert to a shade
            average_color /= (256/MAX_SHADES);
            if(average_color >= MAX_SHADES)
                average_color = MAX_SHADES-1;
            
            // Write Output to string
            if(style == "default"){
                ret_string += def_shades[average_color];
            } else if (style == "numbers"){
                ret_string += num_shades[average_color];
            } else {
                ret_string += def_shades[average_color];
            }
            
        }
        
        ret_string += "\n";
        
    }
    
    return ret_string;
    
}


string AsciiPrinter::word1(const string font_type, const string& word){
    
    string sf_1 = get_loc(font_type);
    sf_1 += word[0];
    sf_1 += ".txt";
    
    ifstream pf_1(sf_1);
    
    string pc_1;
    string ret_string;
    
    if(pf_1.is_open()){
        while (getline(pf_1, pc_1)){
            ret_string += pc_1;
            ret_string += "\n";
        }
        pf_1.close();
    }
    
    return ret_string;
    
}



/*
 string printWord0(const string font_type, const string &word){
 
 }
 
 string printWord1(const string font_type, const string word){
 
 
 
 }
 
 */
