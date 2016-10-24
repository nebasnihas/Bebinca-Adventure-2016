
#include "Command.hpp"

const std::string& Command::getKeyword() const {
    return text;
}

const Command::functionRef& Command::getMethod() const {
    return method;
}




