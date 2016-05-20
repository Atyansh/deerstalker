#pragma once

#include <deque>
#include <mutex>

#include "util\Protos.pb.h"
#include "Globals.h"
#include "Window.h"


class MessageHandler {
public:
	static std::deque<protos::Message> lobbyQueue;
	static std::mutex lobbyLock;

	static std::deque<protos::Message> startQueue;
	static std::mutex startLock;

	static std::deque<protos::Message> gameQueue;
	static std::mutex gameLock;

	static std::deque<protos::Message> endGameQueue;
	static std::mutex endGameLock;

	static void handleLobbyMessages();
	static void handleStartMessages();
	static void handleGameMessages();
	static void handleEndGameMessages();
};

