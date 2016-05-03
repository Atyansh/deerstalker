#include "Game.h"

#include "Serialize\BulletWorldImporter\btBulletWorldImporter.h"

using namespace std::chrono;
using namespace std::this_thread;

Game::Game() : idCounter_(1) {
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
	messageQueue_.push_back(msg);
}

int Game::size() {
	return clients_.size();
}

int Game::generateId() {
	return idCounter_++;
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

		world_->addRigidBody(body);
	}

	btBulletWorldImporter* fileLoader1 = new btBulletWorldImporter();
	btBulletWorldImporter* fileLoader2 = new btBulletWorldImporter();

	fileLoader1->loadFile("bullet_assets\\Mango.bullet");
	fileLoader2->loadFile("bullet_assets\\Player.bullet");
	mango_body_ = fileLoader1->getRigidBodyByIndex(0);
	player_body_ = fileLoader2->getRigidBodyByIndex(0);

	//std::cerr << "Num Rigid Bodies: " << fileLoader1->getNumCollisionShapes() << std::endl;
	
}

void Game::startGameLoop() {
	// TODO(Atyansh): Make this part of config settings 
	milliseconds interval = milliseconds(33);

	while (true) {
		milliseconds stamp1 = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch());

		world_->stepSimulation(1.f / 30.f, 10);

		while (!messageQueue_.empty()) {
			protos::Message message = messageQueue_.front();
			messageQueue_.pop_front();

			for (int i = 0; i < message.event_size(); i++) {
				auto event = message.event(i);
				if (event.type() == protos::Event_Type_SPAWN) {
					handleSpawnLogic(event);
				}
				else if (event.type() == protos::Event_Type_LMOVE) {
					handleMoveLogic(event);
				}
				else if (event.type() == protos::Event_Type_RMOVE) {
					handleViewLogic(event);
				}
				else if (event.type() == protos::Event_Type_JUMP) {
					handleJumpLogic(event);
				}
				else if (event.type() == protos::Event_Type_SHOOT) {
					handleShootLogic(event);
				}
			}
		}
		sendStateToClients();

		milliseconds stamp2 = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch());

		sleep_for(interval - (stamp2-stamp1));
	}
}

void Game::handleSpawnLogic(protos::Event& event) {
	std::lock_guard<std::mutex> lock(playerMapLock_);
	std::cerr << "SPAWN HAPPENED" << std::endl;
	Player* player = Player::createNewPlayer(event.clientid(), generateId(), player_body_->getCollisionShape());
	playerMap_[event.clientid()] = player;
	world_->addRigidBody(player);
}



void Game::handleJumpLogic(protos::Event& event) {
	Player* player = playerMap_[event.clientid()];

	player->applyCentralImpulse(btVector3(0, 1, 0));
}

void Game::handleShootLogic(protos::Event& event) {
	std::lock_guard<std::mutex> lock(playerMapLock_);
	std::cerr << "SHOOT HAPPENED" << std::endl;

	if (playerMap_.find(event.clientid()) == playerMap_.end()) {
		std::cerr << "No Client found" << std::endl;
		return;
	}

	Player* player = playerMap_[event.clientid()];

	Bullet* bullet = Bullet::createNewBullet(generateId(), player, mango_body_->getCollisionShape());
	itemList_.push_back(bullet);
	world_->addRigidBody(bullet);
}

std::tuple<btVector3, btScalar> Game::getRotation(protos::Event_Direction direction,Player * player) {
	btVector3 rotAxis = direction == protos::Event_Direction_LEFT || direction == protos::Event_Direction_RIGHT ? player->getUpAxis() : player->getSideAxis();
	btScalar angle = (direction == protos::Event_Direction_LEFT || direction == protos::Event_Direction_DOWN ? -1 : 1)* ROT_DELTA;
	return std::make_tuple(rotAxis, angle);
}

void Game::handleMoveLogic(protos::Event& event) {
	Player* player = playerMap_[event.clientid()];

	switch (event.direction()) {
	case (protos::Event_Direction_RIGHT):
		std::cerr << "MOVE RIGHT" << std::endl;
		player->applyCentralForce(btVector3(10, 0, 0));
		break;
	case (protos::Event_Direction_LEFT):
		std::cerr << "MOVE LEFT" << std::endl;
		player->applyCentralForce(btVector3(-10, 0, 0));
		break;
	case (protos::Event_Direction_UP):
		std::cerr << "MOVE UP" << std::endl;
		player->applyCentralForce(btVector3(0, 10, 0));
		break;
	case (protos::Event_Direction_DOWN):
		std::cerr << "MOVE DOWN" << std::endl;
		player->applyCentralForce(btVector3(0, -10, 0));
		break;
	case (protos::Event_Direction_FORWARD):
		std::cerr << "MOVE FORWARD" << std::endl;
		player->applyCentralForce(btVector3(0, 0, -10));
		break;
	case (protos::Event_Direction_BACKWARD):
		std::cerr << "MOVE BACKWARD" << std::endl;
		player->applyCentralForce(btVector3(0, 0, 10));
		break;
	}
}

void Game::handleViewLogic(protos::Event& event) {
	Player* player = playerMap_[event.clientid()];
	auto rotData = getRotation(event.direction(), player);

	auto transform = btMatrix3x3();	
	transform.setRotation(btQuaternion(std::get<0>(rotData), std::get<1>(rotData)));
	btVector3 nLook = transform*(player->getLookAt());
	player->setLookAt(&nLook);
}

void Game::sendStateToClients() {
	std::lock_guard<std::mutex> lock(playerMapLock_);

	protos::Message message;

	for (auto& pair : playerMap_) {
		btTransform transform;
		pair.second->getMotionState()->getWorldTransform(transform);

		btScalar glm[16] = {};

		transform.getOpenGLMatrix(glm);

		auto* gameObject = message.add_gameobject();
		gameObject->set_id(pair.first);
		gameObject->set_type(protos::Message_GameObject_Type_PLAYER);

		for (auto v : glm) {
			gameObject->add_matrix(v);
		}
	}

	for (auto* item : itemList_) {
		btTransform transform;
		item->getMotionState()->getWorldTransform(transform);

		btScalar glm[16] = {};

		transform.getOpenGLMatrix(glm);

		auto* gameObject = message.add_gameobject();
		gameObject->set_id(item->getId());
		gameObject->set_type(protos::Message_GameObject_Type_BULLET);

		for (auto v : glm) {
			gameObject->add_matrix(v);
		}
	}

	for (auto client : clients_) {
		client->deliver(message);
	}
}
