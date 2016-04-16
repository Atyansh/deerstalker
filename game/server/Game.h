#pragma once

#include "Client.h"

class Game {
public:
	Game();

	void join(client_ptr client);
	void remove(client_ptr client);
	void deliver(protos::TestEvent msg);
	int size();

private:
	std::set<client_ptr> clients_;
	int requiredPlayers_;
	bool gameStarted_;
};