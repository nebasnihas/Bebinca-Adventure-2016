
#include "Command.hpp"


Command::Command(Command::Type type, string text, Command::functionRef method) {
    this->type = type;
    this->text = text;
    this->method = method;
}
std::string Command::getText() {
    return this->text;
}

Command::functionRef Command::getMethod() {
    return method;
}




