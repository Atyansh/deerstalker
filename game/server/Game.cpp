#include "Game.h"

using namespace std::chrono;
using namespace std::this_thread;

Game::Game() {
}

void Game::join(client_ptr client) {
	clients_.insert(client);
}

void Game::remove(client_ptr client) {
	std::lock_guard<std::mutex> lock(playerMapLock_);
	ClientId clientId = client->getClientId();
	clients_.erase(client);
	auto iter = playerMap_.find(clientId);
	if (iter != playerMap_.end()) {
		playerMap_.erase(iter);
	}
}

void Game::deliver(protos::TestEvent msg) {
	eventQueue_.push_back(msg);
	//for (auto client : clients_) {
	//	client->deliver(msg);
	//}
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

		while (!eventQueue_.empty()) {
			protos::TestEvent event = eventQueue_.front();
			eventQueue_.pop_front();

			if (event.type() == event.SPAWN) {
				handleSpawnLogic(event);
			}
			else if (event.type() == event.MOVE) {
				handleMoveLogic(event);
			}

			sendStateToClients();
		}

		milliseconds stamp2 = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch());

		// TODO(Atyansh): Verify this works
		sleep_for(interval - (stamp2-stamp1));
	}
}

void Game::handleSpawnLogic(protos::TestEvent& event) {
	std::lock_guard<std::mutex> lock(playerMapLock_);
	playerMap_[event.clientid()] = Player::createNewPlayer(event.clientid());
}

void Game::handleMoveLogic(protos::TestEvent& event) {
}

void Game::sendStateToClients() {
	std::lock_guard<std::mutex> lock(playerMapLock_);
	std::cerr << "TRIED THIS" << std::endl;

	protos::TestEvent event;

	for (auto& pair : playerMap_) {
		btTransform transform;
		pair.second->getMotionState()->getWorldTransform(transform);

		btScalar glm[16] = {};

		transform.getOpenGLMatrix(glm);

		event.set_type(event.MOVE);

		protos::GameObject* gameObject = event.add_gameobject();
		gameObject->set_id(pair.first);
		for (auto v : glm) {
			gameObject->add_matrix(v);
		}
	}

	for (auto client : clients_) {
		client->deliver(event);
	}
}