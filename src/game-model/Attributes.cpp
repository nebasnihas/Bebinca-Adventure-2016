#include "Attributes.hpp"
#include <algorithm>

Attributes::Attributes() {
    attributes = {
            {"strength", SECONDARY_BASE_NUM},
            {"intelligence", SECONDARY_BASE_NUM},
            {"dexterity", SECONDARY_BASE_NUM},
            {"vitality", VITALITY_BASE_NUM}
    };
}

Attributes::~Attributes() { }

std::string Attributes::getString() const {
    std::string attributesString = "";

    for (const auto& attribute : attributes) {
        attributesString += attribute.first + ": " + std::to_string(attribute.second) + "\n";
    }

    return attributesString;
}

int Attributes::getAttribute(std::string attribute) {
    return attributes[toLower(attribute)];
}

void Attributes::setPrimaryAttribute(std::string primaryAttribute) {
    if (isValidAttribute(primaryAttribute)) {
        attributes[toLower(primaryAttribute)] = PRIMARY_BASE_NUM;
    }
}

void Attributes::increaseAttribute(std::string attribute) {
    if (isValidAttribute(attribute)) {
        attributes[toLower(attribute)]++;
    }
}

bool Attributes::isValidAttribute(std::string attribute) {
    return attributes.find(toLower(attribute)) != attributes.end();
}

std::string Attributes::toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}
