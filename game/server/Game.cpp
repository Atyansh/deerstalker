#include "Game.h"

using namespace std::chrono;
using namespace std::this_thread;

Game::Game() {
}

void Game::join(client_ptr client) {
	clients_.insert(client);
}

void Game::remove(client_ptr client) {
	clients_.erase(client);
}

void Game::deliver(protos::TestEvent msg) {
	for (auto client : clients_) {
		client->deliver(msg);
	}
}

int Game::size() {
	return clients_.size();
}

void Game::initialize() {
}

void Game::startGameLoop() {
	// TODO(Atyansh): Make this part of config settings 
	milliseconds interval = milliseconds(33);

	while (true) {
		milliseconds stamp1 = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch());



		milliseconds stamp2 = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch());

		// TODO(Atyansh): Verify this works
		sleep_for(interval - (stamp2-stamp1));
	}
}