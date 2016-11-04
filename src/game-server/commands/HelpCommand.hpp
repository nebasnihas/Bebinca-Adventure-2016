#ifndef ADVENTURE2016_HELPCOMMAND_HPP
#define ADVENTURE2016_HELPCOMMAND_HPP

#include <Controller.hpp>
#include "Command.hpp"

class HelpCommand : public Command {
public:
    HelpCommand(Controller& controller) {};

    virtual std::unique_ptr<MessageBuilder> execute(const gsl::span<std::string, -1> arguments,
                                                    const PlayerInfo& player) override {
        return nullptr;
    }
};


#endif //ADVENTURE2016_HELPCOMMAND_HPP
