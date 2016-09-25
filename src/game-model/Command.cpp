
#include "Command.hpp"


Command::Command(Command::Type type, std::string text) {
    this->type = type;
    this->text = text;
}

std::string Command::getText() {
    return this->text;
}
