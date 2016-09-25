#ifndef ADVENTURE2016_COMMAND_H
#define ADVENTURE2016_COMMAND_H


#include <vector>
#include <string>

class Command {
public:
    enum Type {
        AREA,
        CHARACTER,
        ENTITY,
        GAME
    };

    Command(Type type, std::string text);
    std::string getText();

private:
    Type type;
    std::string text;
};


#endif //ADVENTURE2016_COMMAND_H
