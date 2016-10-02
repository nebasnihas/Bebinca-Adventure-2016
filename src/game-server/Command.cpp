
#include "Command.hpp"

std::string Command::getKeyword() const {
    return this->text;
}

Command::functionRef Command::getMethod() const {
    return method;
}




