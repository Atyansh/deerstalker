#pragma once

#include <chrono>
#include <thread>

#include "Client.h"
#include "World.h"

class Game {
public:
	Game();

	void join(client_ptr client);
	void remove(client_ptr client);
	void deliver(protos::TestEvent msg);
	int size();

	void initialize();
	void startGameLoop();

private:
	std::set<client_ptr> clients_;
	std::unique_ptr<World> world_;


};