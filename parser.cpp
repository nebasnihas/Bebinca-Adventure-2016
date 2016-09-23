//parser.h

//Zachary Cesaretti
//301235784
//CMPT 373

#include "parser.h"
#include <iostream>
#include <string>
#include <vector>

//Create a Dictionary of Valid words in these 4 categories
enum class en_DIRS {NORTH, EAST,SOUTH, WEST, UP, DOWN };
enum class en_ROOMS {CAVE, KITCHEN};
enum class en_VERBS {GET, DROP, USE, OPEN, CLOSE, EXAMINE, INVENTORY, LOOK};
enum class en_NOUNS {SWORD, GOLD, TREE, BUSH};

//Convert inputted command to uppercase
string toUpper(string input){
	std::transform(input.begin(), input.end(),input.begin(), ::toupper);
	return input;
}

//split up string into individual words, return a vector of words
vector<string> stringSplitter(string input){

	return words;
}

//class for verbs
class Verb {
	public:
		string verb;
		Words();
		~Words();
};

//class for rooms
class Room
  {
  public:
  string description;
  int exits_to_Room[en_DIRS];
  Room ();
  ~Room ();
  
  };

//class for nouns	  
class Noun
  {
  public:
     string word;
	 string description;
	 bool can_carry;
	 Room;
	 Noun (); 
	 ~Noun (); 
  
  };

//A collection of rooms with their exits, defining what is allowed
void set_rooms(vector<Room> &rms){
	rms[CAVE].description.assign("Dark grungy cave, slime on the walls, stack of gold in the middle");
	rms[CAVE].exits_to_Room[NORTH] = NONE;
	rms[CAVE].exits_to_Room[EAST] = NONE;
	rms[CAVE].exits_to_Room[SOUTH] = KITCHEN;
	rms[CAVE].exits_to_Room[WEST] = NONE;
	rms[CAVE].exits_to_Room[UP] = NONE;
	rms[CAVE].exits_to_Room[DOWN] = NONE;


	rms[KITCHEN].description.assign("Beautiful and shiny, with a wicked cool knife");
	rms[KITCHEN].exits_to_Room[NORTH] = CAVE;
	rms[KITCHEN].exits_to_Room[EAST] = NONE;
	rms[KITCHEN].exits_to_Room[SOUTH] = NONE;
	rms[KITCHEN].exits_to_Room[WEST] = NONE;
	rms[KITCHEN].exits_to_Room[UP] = NONE;
	rms[KITCHEN].exits_to_Room[DOWN] = NONE;		

}

//enum class en_NOUNS {SWORD, GOLD, TREE, BUSH};
 void set_nouns(Noun& nns){
 	nns[SWORD].word = "DOOR";
 	nns[SWORD].description = "A shiny blade of doom";
 	nns[SWORD].can_cary = TRUE;
 	nns[SWORD].Room = KITCHEN;

 	nns[GOLD].word = "GOLD";
 	nns[GOLD].description = "Shiny, beautiful gold";
 	nns[GOLD].can_cary = TRUE;
 	nns[GOLD].Room = CAVE;


 }

//enum class en_VERBS {GET, DROP, USE, OPEN, CLOSE, EXAMINE, INVENTORY, LOOK};
 void set_verbs(Verbs){
	vbs[GET].word = "GET";
	vbs[DROP].word = "DROP";
	vbs[USE].word = "USE";
	vbs[OPEN].word = "OPEN";
	vbs[CLOSE].word = "CLOSE";
	vbs[EXAMINE].word = "EXAMINE";
	vbs[INVENTORY].word = "INVENTORY";
	vbs[LOOK].word = "LOOK";
 }


//Parser
bool parser(string& location, vector<string> words){
	for(string word : words){
		if(word == )
	}

}














