#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <assert.h>


class Object {

public:

    //*---------------
    // Constructors
    //---------------*

    Object(	const std::vector<std::string>& attributes,
			int cost,
			const std::vector<std::string>& extra,
			const std::string& ID,
			const std::string& type,
			const std::vector<std::string>& keywords,
			const std::string& description,
			const std::string& name,
			const std::vector<std::string>& wearflags,
			int weight);

    //*---------------
    // Getters
    //---------------*


    std::string getID() const;
    std::string getDescription() const;
    std::vector<std::string> getAttributes() const;
	int getCost() const;
	std::vector<std::string> getExtra() const;
    std::string getType() const; //Maybe we can have a subclass for each type of object
    std::vector<std::string> getKeywords() const;
    std::string getName() const;
    std::vector<std::string> getWearflags() const;
    int getWeight() const;



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

/*
 *  VARIOUS OBJECT EXCEPTIONS
 */

struct InvalidObjectException : public std::exception
{

    virtual const char* what() const throw()
    {
        return "This is an invalid object.";
    }

}; //Invalid Object exception

#endif