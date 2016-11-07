#include "GameFunctions.hpp"
#include "commands/CommandList.hpp"


GameFunctions::GameFunctions(Controller& controller)
        : controller{controller}, gameModel{controller.getGameModel()},
          listExitsCommand{gameModel},
          listPlayersCommand{gameModel},
          lookCommand{gameModel},
          moveCommand{gameModel},
          sayCommand{controller},
          shoutCommand{gameModel, controller},
          whisperCommand{controller},
          attackCommand{gameModel, controller},
          castCommand{gameModel, controller} {
    registerCommands();
}

void GameFunctions::registerCommands() {
    controller.registerCommand(COMMAND_EXITS, listExitsCommand);
    controller.registerCommand(COMMAND_PLAYERS, listPlayersCommand);
    controller.registerCommand(COMMAND_LOOK, lookCommand);
    controller.registerCommand(COMMAND_MOVE, moveCommand);
    controller.registerCommand(COMMAND_SAY, sayCommand);
    controller.registerCommand(COMMAND_SHOUT, shoutCommand);
    controller.registerCommand(COMMAND_WHISPER, whisperCommand);
    controller.registerCommand(COMMAND_ATTACK, attackCommand);
    controller.registerCommand(COMMAND_CAST, castCommand);
}



