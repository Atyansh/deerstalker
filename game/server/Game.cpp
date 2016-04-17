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
	world_ = World::createNewWorld();
	world_->setGravity(btVector3(0, -10, 0));

	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -56, 0));

	{
		btScalar mass(0.);

		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		world_->addRigidBody(body);
	}
}

void Game::startGameLoop() {
	// TODO(Atyansh): Make this part of config settings 
	milliseconds interval = milliseconds(33);

	while (true) {
		milliseconds stamp1 = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch());

		world_->stepSimulation(1.f / 60.f, 10);

		while (!eventQueue_.empty()) {
			protos::TestEvent event = eventQueue_.front();
			eventQueue_.pop_front();

			if (event.type() == event.SPAWN) {
				handleSpawnLogic(event);
			}
			else if (event.type() == event.MOVE) {
				handleMoveLogic(event);
			}
		}
		sendStateToClients();

		milliseconds stamp2 = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch());

		// TODO(Atyansh): Verify this works
		sleep_for(interval - (stamp2-stamp1));
	}
}

void Game::handleSpawnLogic(protos::TestEvent& event) {
	std::lock_guard<std::mutex> lock(playerMapLock_);
	Player* player = Player::createNewPlayer(event.clientid());
	playerMap_[event.clientid()] = player;
	world_->addRigidBody(player);
}

void Game::handleMoveLogic(protos::TestEvent& event) {
	Player* player = playerMap_[event.clientid()];
	
	player->setActivationState(1);

	switch (event.direction()) {
	case (protos::RIGHT) :
		std::cerr << "MOVE RIGHT" << std::endl;
		player->applyCentralForce(btVector3(100, 0, 0));
		break;
	case (protos::LEFT) :
		std::cerr << "MOVE LEFT" << std::endl;
		player->applyCentralForce(btVector3(-100, 0, 0));
		break;
	case (protos::UP) :
		std::cerr << "MOVE UP" << std::endl;
		player->applyCentralForce(btVector3(0, 100, 0));
		break;
	case (protos::DOWN) :
		std::cerr << "MOVE DOWN" << std::endl;
		player->applyCentralForce(btVector3(0, -100, 0));
		break;
	case (protos::FORWARD) :
		std::cerr << "MOVE FORWARD" << std::endl;
		player->applyCentralForce(btVector3(0, 0, -100));
		break;
	case (protos::BACKWARD) :
		std::cerr << "MOVE BACKWARD" << std::endl;
		player->applyCentralForce(btVector3(0, 0, 100));
		break;
	}
}

void Game::sendStateToClients() {
	std::lock_guard<std::mutex> lock(playerMapLock_);

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