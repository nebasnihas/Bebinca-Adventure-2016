#ifndef ADVENTURE2016_SAYCOMMAND_HPP
#define ADVENTURE2016_SAYCOMMAND_HPP

#include <commands/Command.hpp>
#include <Controller.hpp>

#include <gsl/gsl>

class SayCommand : public Command {
public:
    SayCommand(Controller& controller);

    virtual std::unique_ptr<MessageBuilder> execute(const gsl::span<std::string, -1> arguments,
                                                    const PlayerInfo& player) override;

private:
    Controller& controller;
};


#endif //ADVENTURE2016_SAYCOMMAND_HPP
