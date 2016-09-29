#ifndef GAMESERVER_COMMANDHANDLER_H
#define GAMESERVER_COMMANDHANDLER_H

#include <string>
#include <map>
#include <functional>

#include "server.h"
#include "messageSender.h"

using namespace networking;

void processCommand(const Message& message, MessageSender& messageSender);


#endif
