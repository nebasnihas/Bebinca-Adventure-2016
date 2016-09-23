#include "Character.hpp"

Character::Character() {
	this->id = 0;
	this->areaID = 0;
}

Character::Character(std::string name) {
	this->name = name;
	this->id = 0;
	this->areaID = 0;
}

