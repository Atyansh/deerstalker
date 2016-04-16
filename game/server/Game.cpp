#include "Game.h"

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