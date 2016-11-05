#ifndef ADVENTURE2016_CASTCOMMAND_H
#define ADVENTURE2016_CASTCOMMAND_H

#include <commands/Command.hpp>
#include <game/GameModel.hpp>

class CastCommand : public Command {
public:
    CastCommand(GameModel& gameModel);
    virtual std::unique_ptr<MessageBuilder> execute(const gsl::span<std::string, -1> arguments,
                                                    const PlayerInfo& player) override;

private:
    GameModel& gameModel;

};


#endif //ADVENTURE2016_CASTCOMMAND_H
