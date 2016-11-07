#include "AttackCommand.hpp"
#include <iostream>

AttackCommand::AttackCommand(GameModel &gameModel, Controller &controller)
        : gameModel{gameModel}, controller{controller} {
}

std::unique_ptr<MessageBuilder>
AttackCommand::execute(const gsl::span<std::string, -1> arguments, const PlayerInfo &player) {
    if (arguments.empty()) {
        return setCombatAction(player);
    }

    auto targetID = arguments[0];
    auto targetClient = controller.getClientID(targetID);

    if (!targetClient) {
        std::string invalidPlayer = targetID + " " + NOT_FOUND;
        return buildPlayerMessage(player, invalidPlayer);
    }

    if (gameModel.characterIsInCombat(player.playerID)) {
        return buildPlayerMessage(player, PLAYER_IN_BATTLE);
    }

    if (gameModel.characterIsInCombat(targetID)) {
        return buildPlayerMessage(player, TARGET_IN_BATTLE);
    }

    if (!gameModel.engageCharacterInCombat(player.playerID, targetID)) {
        std::cerr << "Invalid state in AttackCommand";
    }
    return DisplayMessageBuilder{COMBAT_ENGAGED + " " + gameModel.getCharacterByID(targetID)->getName()}
            .addClient(player.clientID)
            .setSender(DisplayMessageBuilder::SENDER_SERVER);
}


std::unique_ptr<MessageBuilder>
AttackCommand::setCombatAction(const PlayerInfo &player) {
    if (gameModel.getCharacterByID(player.playerID)->getState() != CharacterState::BATTLE) {
        return buildPlayerMessage(player, NOT_IN_COMBAT);
    }

    gameModel.setCombatAction(player.playerID, "attack");
    return buildPlayerMessage(player, SET_COMBAT_ACTION);
}

