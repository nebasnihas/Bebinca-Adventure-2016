#ifndef ADVENTURE2016_STRINGUTILS_HPP
#define ADVENTURE2016_STRINGUTILS_HPP

#include <string>
#include "boost/algorithm/string.hpp"

static inline std::pair<std::string, std::string> separateFirstWord(const std::string text) noexcept
{
    const std::string delim = " \t";
    auto delimPos = text.find_first_of(delim);

    std::string firstWord;
    std::string rest;

    if (delimPos == std::string::npos) {
        firstWord = text;
    } else {
        firstWord = text.substr(0, delimPos);
        rest = text.substr(delimPos + 1);
    }

    return std::pair<std::string, std::string>(firstWord, rest);
};

static inline std::vector<std::string> splitString(const std::string& text)
{
    std::vector<std::string> tokens;
    boost::split(tokens, text, boost::is_any_of(" \t"));
    return tokens;
}

#endif //ADVENTURE2016_STRINGUTILS_HPP
