#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <vector>

class Entity {

public:

    Entity(std::vector<std::string> attributes, int cost, std::vector<std::string> extra, std::string ID, std::string type, std::vector<std::string> keywords, std::string description, std::string name, std::vector<std::string> wearflags, int weight);


    std::string getID();
    std::string getDescription();
    std::vector<std::string> getAttributes();
	int getCost();
	std::vector<std::string> getExtra();
    std::string getType(); //Maybe we can have a subclass for each type of object
    std::vector<std::string> getKeywords();
    std::string getName();
    std::vector<std::string> getWearflags();
    int getWeight();



private:

	std::vector<std::string> attributes;
	int cost;
	std::vector<std::string> extra;
    std::string ID;
    std::string type; //Maybe we can have a subclass for each type of object
    std::vector<std::string> keywords;
    std::string description;
    std::string name;
    std::vector<std::string> wearflags;
    int weight;
    
};

#endif