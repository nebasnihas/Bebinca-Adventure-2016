
#include "Command.hpp"

const std::string& Command::getKeyword() const {
    return keyword;
}

const Command::functionRef& Command::getMethod() const {
    return method;
}

const std::string& Command::getDesc() const {
    return desc;
}

const std::string& Command::getUsage() const {
    return usage;
}

const std::vector<std::string> Command::getBindings() const {
    return bindings;
}

void Command::setDesc(const std::string& desc) {
    this->desc = desc;
}

void Command::setUsage(const std::string& usage) {
    this->usage = usage;
}

void Command::addBinding(const std::string& binding) {
    bindings.push_back(binding);
}



