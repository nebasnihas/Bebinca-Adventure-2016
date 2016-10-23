#include "Entity.hpp"

#include <string>
#include <vector>

Entity::Entity(std::vector<std::string> attributes, int cost, std::vector<std::string> extra, std::string ID, std::string type, std::vector<std::string> keywords, std::string description, std::string name, std::vector<std::string> wearflags, int weight)
    : attributes(attributes)
    , cost(cost)
    , extra(extra)
    , ID(ID)
    , type(type)
    , keywords(keywords)
    , description(description)
    , name(name)
    , wearflags(wearflags)
    , weight(weight)
{ }

    std::string Entity::getDescription()
    {
    	return description;
    }

    std::vector<std::string> Entity::getAttributes()
    {
    	return attributes;
    }

	int Entity::getCost()
	{
		return cost;
	}

	std::vector<std::string> Entity::getExtra()
	{
		return extra;
	}

    std::string Entity::getID()
    {
    	return ID;
    }

    std::string Entity::getType()
    {
    	return type;
    }
     //Maybe we can have a subclass for each type of object
    std::vector<std::string> Entity::getKeywords()
    {
    	return keywords;
    }

    std::string Entity::getName()
    {
    	return name;
    }

    std::vector<std::string> Entity::getWearflags()
    {
    	return wearflags;
    }

    int Entity::getWeight()
    {
    	return weight;
    }