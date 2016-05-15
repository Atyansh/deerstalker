#include "Game.h"

#include "Serialize\BulletWorldImporter\btBulletWorldImporter.h"

using namespace std::chrono;
using namespace std::this_thread;

Game::Game() {
}

Game::~Game() {
	// TODO(Atyansh): Deal with cleaning up game objects.
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

void Game::deliver(protos::Message msg) {
	std::lock_guard<std::mutex> lock(queueLock_);
	messageQueue_.push_back(msg);
}

int Game::size() {
	return clients_.size();
}

void Game::initialize() {
	world_ = World::createNewWorld();
	world_->setGravity(btVector3(0, -10, 0));

	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
	
	//btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 5);
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

		//world_->addRigidBody(body);
	}

	btBulletWorldImporter* playerLoader = new btBulletWorldImporter();
	playerLoader->loadFile("bullet_assets\\Player.bullet");
	playerBody_ = playerLoader->getRigidBodyByIndex(0);

	btBulletWorldImporter* mangoLoader = new btBulletWorldImporter();
	mangoLoader->loadFile("bullet_assets\\mango.bullet");
	mangoBody_ = playerLoader->getRigidBodyByIndex(0);

	btBulletWorldImporter* worldLoader = new btBulletWorldImporter(world_);
	worldLoader->loadFile("bullet_assets\\Construction_Stage_Bullet_Scaled.bullet");
}

void Game::startGameLoop() {
	// TODO(Atyansh): Make this part of config settings 
	milliseconds interval = milliseconds(33);
	int frameCounter = 0;

	while (true) {
		milliseconds stamp1 = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch());

		world_->stepSimulation(1.f / 30.f, 10);

		queueLock_.lock();
		while (!messageQueue_.empty()) {
			protos::Message& message = messageQueue_.front();

			for (int i = 0; i < message.event_size(); i++) {
				auto event = &message.event(i);
				if (event->type() == protos::Event_Type_SPAWN) {
					handleSpawnLogic(event);
				}
				else if (event->type() == protos::Event_Type_MOVE) {
					handleMoveLogic(event);
				}
				else if (event->type() == protos::Event_Type_JUMP) {
					handleJumpLogic(event);
				}
			}

			messageQueue_.pop_front();
		}
		queueLock_.unlock();

		sendStateToClients();

		milliseconds stamp2 = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch());

		sleep_for(interval - (stamp2-stamp1));

		if (frameCounter > 300) {
			//spawnNewHat();
			frameCounter = 0;
		}
		else {
			frameCounter++;
		}
	}
}

void Game::handleSpawnLogic(const protos::Event* event) {
	std::lock_guard<std::mutex> lock(playerMapLock_);
	std::cerr << "SPAWN HAPPENED" << std::endl;
	Player* player = new Player(playerBody_);
	playerMap_[event->clientid()] = player;
	world_->addRigidBody(player->getController()->getRigidBody());
	world_->addAction(player->getController());
}

void Game::handleMoveLogic(const protos::Event* event) {
	Player* player = playerMap_[event->clientid()];

	double x = event->cameravector(0);
	double y = event->cameravector(1);
	double z = event->cameravector(2);

	switch (event->direction()) {
	case (protos::Event_Direction_RIGHT) :
		std::cerr << "MOVE RIGHT" << std::endl;
		player->getController()->playerStep(world_, btVector3(z, -y, -x));
		//player->applyCentralForce(btVector3(10, 0, 0));
		break;
	case (protos::Event_Direction_LEFT) :
		std::cerr << "MOVE LEFT" << std::endl;
		player->getController()->playerStep(world_, btVector3(-z, y, x));
		//player->applyCentralForce(btVector3(-10, 0, 0));
		break;
	case (protos::Event_Direction_UP) :
		std::cerr << "MOVE UP" << std::endl;
		//player->applyCentralForce(btVector3(0, 10, 0));
		player->getController()->getRigidBody()->applyCentralForce(btVector3(0, 10, 0));
		break;
	case (protos::Event_Direction_DOWN) :
		std::cerr << "MOVE DOWN" << std::endl;
		//player->applyCentralForce(btVector3(0, -10, 0));
		player->getController()->getRigidBody()->applyCentralForce(btVector3(0, -10, 0));
		break;
	case (protos::Event_Direction_FORWARD) :
		std::cerr << "MOVE FORWARD" << std::endl;
		player->getController()->playerStep(world_, btVector3(-x, -y, -z));
		//player->applyCentralForce(btVector3(0, 0, -10));
		break;
	case (protos::Event_Direction_BACKWARD) :
		std::cerr << "MOVE BACKWARD" << std::endl;
		player->getController()->playerStep(world_, btVector3(x, y, z));
		//player->applyCentralForce(btVector3(0, 0, 10));
		break;
	}
	std::cerr << "x: " << x << "\ty: " << y << "\tz:" << z << std::endl;
}

void Game::handleJumpLogic(const protos::Event* event) {
	Player* player = playerMap_[event->clientid()];
	player->getController()->jump();
}

void Game::sendStateToClients() {
	std::lock_guard<std::mutex> lock(playerMapLock_);

	protos::Message message;

	for (auto& pair : playerMap_) {
		btTransform transform;
		pair.second->getController()->getRigidBody()->getMotionState()->getWorldTransform(transform);
		btScalar glm[16] = {};

		transform.getOpenGLMatrix(glm);

		auto* gameObject = message.add_gameobject();
		gameObject->set_type(protos::Message_GameObject_Type_PLAYER);
		gameObject->set_id(pair.first);
		for (auto v : glm) {
			gameObject->add_matrix(v);
		}
	}

	for (auto* hat : hatSet_) {
		btTransform transform;
		hat->getMotionState()->getWorldTransform(transform);

		btScalar glm[16] = {};

		transform.getOpenGLMatrix(glm);

		auto* gameObject = message.add_gameobject();
		gameObject->set_type(protos::Message_GameObject_Type_HAT);
		gameObject->set_id(hat->getHatId());
		for (auto v : glm) {
			gameObject->add_matrix(v);
		}
	}

	for (auto client : clients_) {
		client->deliver(message);
	}
}

void Game::spawnNewHat() {
	Hat* hat = Hat::createNewHat(Hat::WIZARD_HAT);
	hatSet_.insert(hat);
	world_->addRigidBody(hat);
}