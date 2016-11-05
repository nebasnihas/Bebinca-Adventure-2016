#ifndef ADVENTURE2016_WHISPERCOMMAND_HPP
#define ADVENTURE2016_WHISPERCOMMAND_HPP

#include <commands/Command.hpp>
#include <Controller.hpp>


class WhisperCommand : public Command {
public:
    WhisperCommand(Controller& controller);

    virtual std::unique_ptr<MessageBuilder> execute(const gsl::span<std::string, -1> arguments,
                                                    const PlayerInfo& player) override;

private:
    Controller& controller;
};


#endif //ADVENTURE2016_WHISPERCOMMAND_HPP
