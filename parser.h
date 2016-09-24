//parser.h

//Zachary Cesaretti
//301235784
//CMPT 373

#include <string>
#include <vector>

using std::string;
using std::vector;

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

string toUpperFunction(string input);

vector<string> stringSplitter(string input);

bool parser(string& location, vector<string> words);

#endif