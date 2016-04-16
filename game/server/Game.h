#pragma once
#include <iostream>

#include <chrono>
#include <thread>
#include <deque>

#include "Client.h"
#include "World.h"
#include "Player.h"

#include <mutex>

class Game {
public:
	Game();

	void join(client_ptr client);
	void remove(client_ptr client);
	void deliver(protos::TestEvent msg);
	int size();

	void initialize();
	void startGameLoop();

	void handleSpawnLogic(protos::TestEvent& event);
	void handleMoveLogic(protos::TestEvent& event);

	void sendStateToClients();

private:
	std::set<client_ptr> clients_;
	std::unique_ptr<World> world_;

	std::deque<protos::TestEvent> eventQueue_;
	std::unordered_map<ClientId, Player*> playerMap_;
	std::mutex playerMapLock_;
};