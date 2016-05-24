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
	//TODO CLIENTID IS AN EMPTY IDEA
	idGen_ = idGen_>client->getClientId() ? idGen_ : client->getClientId();
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

unsigned int Game::generateId() {
	return ++idGen_;
}

void Game::deliver(protos::Message msg) {
	queueLock_.lock();
	messageQueue_.push_back(msg);
	queueLock_.unlock();
}

int Game::size() {
	return clients_.size();
}

void Game::initialize() {
	world_ = World::createNewWorld();
	world_->setGravity(btVector3(0, -10, 0));

	btBulletWorldImporter* playerLoader = new btBulletWorldImporter();
	playerLoader->loadFile("bullet_assets\\Player.bullet");
	playerBody_ = playerLoader->getRigidBodyByIndex(0);

	btBulletWorldImporter* mangoLoader = new btBulletWorldImporter();
	mangoLoader->loadFile("bullet_assets\\mango.bullet");
	mangoBody_ = playerLoader->getRigidBodyByIndex(0);

	btBulletWorldImporter* worldLoader = new btBulletWorldImporter(world_);
	worldLoader->loadFile("bullet_assets\\Construction_Stage_Bullet_Scaled.bullet");
	std::cerr << "AJ 1" << std::endl;
}

void Game::startGameLoop() {
	// TODO(Atyansh): Make this part of config settings 
	milliseconds interval = milliseconds(33);
	int frameCounter = 0;
	int maxHat = 2;
	int hatP = 0;
	while (true) {
		milliseconds stamp1 = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch());

		world_->stepSimulation(1.f / 30.f, 10);

		queueLock_.lock();
		while (!messageQueue_.empty()) {
			protos::Message message = messageQueue_.front();

			for (int i = 0; i < message.event_size(); i++) {
				auto event = message.event(i);
				if (event.type() == protos::Event_Type_SPAWN) {
					handleSpawnLogic(&event);
				}
				
				else if (playerMap_.count(event.clientid()) == 0) {
					//TODO WHAT CAN THE DEAD DO
					std::cout << "WHAT CAN THE DEAD DO\n";
					continue;
				}
				else if (event.type() == protos::Event_Type_MOVE) {
					handleMoveLogic(&event);
				}
				else if (event.type() == protos::Event_Type_JUMP) {
					handleJumpLogic(&event);
				}
				else if (event.type() == protos::Event_Type_EQUIP) {
					handleEquipLogic(&event);
				}
				else if (event.type() == protos::Event_Type_DQUIP) {
					handleDquipLogic(&event);
				}
				else if (event.type() == protos::Event_Type_SHOOT) {
					handleShootLogic(&event);
				}
				else if (event.type() == protos::Event_Type_HATR) {
					handlePrimaryHatLogic(&event);
				}
				else if (event.type() == protos::Event_Type_HATL) {
					handleSecondaryHatLogic(&event);
				}
				else if (event.type() == protos::Event_Type_PUNCH) {
					handlePunchLogic(&event);
				}
			}
			messageQueue_.pop_front();
		}
		queueLock_.unlock();

		handleReSpawnLogic();

		sendStateToClients();

		milliseconds stamp2 = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch());

		sleep_for(interval - (stamp2-stamp1));

		if (frameCounter > 300 && maxHat > hatP) {
			spawnNewHat();
			hatP++;
			frameCounter = 0;
		}
		else {
			frameCounter++;
		}
	}
}

void Game::handleShootLogic(const protos::Event* event) {
	std::cerr << "SHOOT HAPPENED" << std::endl;
	Player* player = playerMap_[event->clientid()];
	Bullet * bull = Bullet::createNewBullet(generateId(), mangoBody_->getCollisionShape(), player->getId());
	player->setProjectile(bull,bull->getVelocity());
	shots_[bull->getId()] = bull;
	world_->addRigidBody(bull);
}

void Game::handleDquipLogic(const protos::Event* event) {
	Player* player = playerMap_[event->clientid()];
	Hat * oHat = player->setHat(0);
	if (oHat == 0) {
		return;
	}

	oHat->playerId_ = 0;

	//TODO BETTER
	//MAYBE SET AS CONFIG
	player->setProjectile(oHat, 10);

	hatSet_.emplace(oHat);
	world_->addRigidBody(oHat);
}

void Game::handleSpawnLogic(const protos::Event* event) {
	std::lock_guard<std::mutex> lock(playerMapLock_);
	std::cerr << "SPAWN HAPPENED" << std::endl;
	Player* player = new Player(playerBody_, event->clientid(), 3);
	playerMap_[event->clientid()] = player;
	btRigidBody* body = player->getController()->getRigidBody();
	playerSet_.emplace(body);
	world_->addRigidBody(body);
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

		Hat* hat = pair.second->getHat();

		gameObject->set_hattype(pair.second->getHatType());
		gameObject->set_type(protos::Message_GameObject_Type_PLAYER);
		gameObject->set_id(pair.first);
		for (auto v : glm) {
			gameObject->add_matrix(v);
		}
	}

	for (auto* hat : hatRemovedSet_) {
		auto* event = message.add_event();
		event->set_clientid(hat->playerId_);
		event->set_type(protos::Event_Type_EQUIP);
		event->set_hatid(hat->getHatId());
	}

	hatRemovedSet_.clear();

	for (auto* hat : hatSet_) {
		btTransform transform;
		hat->getMotionState()->getWorldTransform(transform);

		btScalar glm[16] = {};

		transform.getOpenGLMatrix(glm);

		auto* gameObject = message.add_gameobject();
		gameObject->set_type(protos::Message_GameObject_Type_HAT);
		gameObject->set_hattype(hat->getHatType());
		gameObject->set_id(hat->getHatId());
		for (auto v : glm) {
			gameObject->add_matrix(v);
		}
	}

	for (auto& pair : shots_) {
		btTransform transform;
		pair.second->getMotionState()->getWorldTransform(transform);

		btScalar glm[16] = {};

		transform.getOpenGLMatrix(glm);

		auto* gameObject = message.add_gameobject();
		gameObject->set_type(protos::Message_GameObject_Type_BULLET);
		gameObject->set_id(pair.first);
		for (auto v : glm) {
			gameObject->add_matrix(v);
		}
	}

	for (auto client : clients_) {
		client->deliver(message);
	}
}

void Game::spawnNewHat() {
	Hat* hat = Hat::createNewHat(WIZARD_HAT);
	hatSet_.insert(hat);
	world_->addRigidBody(hat);
}

void Game::handleReSpawnLogic() {
	std::list<unsigned int> theDead;
	for (auto it = playerMap_.begin(); it != playerMap_.end(); it++) {
		if (world_->isDead(it->second)) {
			Player * currP = it->second;
			unsigned int pLives = currP->getLives() - 1;
			if (pLives > 0) {
				world_->spawnPlayer(currP);
				currP->setLives(pLives);
				std::cerr << "Player " << currP->getId() << " died\n";
			}
			else {
				//TODO MAYBE DEAL WITH SOME EVENT SHIT
				std::cerr << "Player " << currP->getId() << "is dead forever "<< std::endl;
				theDead.push_back(currP->getId());
				world_->removeRigidBody(currP->getController()->getRigidBody());
			}
			
		}
	}
	
	for (auto id : theDead) {
		playerMap_.erase(id);
	}
}
 
bool Game::canEquip(Player * playa, Hat * hata) {
	//TODO MAYBE SOME BETTER SHIT
	float equipDistance = 5;
	return equipDistance>=playa->getController()->getRigidBody()->getCenterOfMassPosition().distance(hata->getCenterOfMassPosition());
}

void Game::handleEquipLogic(const protos::Event* event) {
	Player * player = playerMap_[event->clientid()];
	std::cout << event->clientid() << " Attempting to equip hat\n";
	Hat* hatToRemove = nullptr;
	Hat* hatToAdd = nullptr;
	for (auto hat : hatSet_) {
		if (canEquip(player, hat)) {
			std::cerr << "Equip success\n";
			Hat* oldHat  = player->setHat(hat);
			hatToRemove = hat;
			if (oldHat != nullptr) {
				btTransform trans;
				hatToAdd = oldHat;
			}
			world_->removeRigidBody(hat);
			break;
		}
	}

	if (hatToAdd) {
		hatToAdd->playerId_ = 0;
		world_->addRigidBody(hatToAdd);
		player->setProjectile(hatToAdd, 10);
		hatSet_.emplace(hatToAdd);
	}
	if (hatToRemove) {
		hatRemovedSet_.emplace(hatToRemove);
		hatToRemove->playerId_ = event->clientid();
		hatSet_.erase(hatToRemove);
	}
}

void Game::handlePunchLogic(const protos::Event* event) {
	Player* player = playerMap_[event->clientid()];
	btCollisionObject* target = player->getController()->getPunchTarget();

	if (target) {
		std::cerr << "SOME TARGET" << std::endl;
		auto search = playerSet_.find(target);
		if (search != playerSet_.end()) {
			std::cerr << "PUNCH DETECTED" << std::endl;
		}
	}
}

void Game::handlePrimaryHatLogic(const protos::Event* event) {
	Player* player = playerMap_[event->clientid()];
	Hat* hat = player->getHat();

	if (!hat) {
		return;
	}

	HatType type = hat->getHatType();

	switch (type) {
	case PROPELLER_HAT:
		propellerUp(player);
		break;
	case WIZARD_HAT:
		handleShootLogic(event);
		break;
	case HARD_HAT:
		break;
	case BEAR_HAT:
		break;
	}
}

void Game::handleSecondaryHatLogic(const protos::Event* event) {
	Player* player = playerMap_[event->clientid()];
	Hat* hat = player->getHat();

	if (!hat) {
		return;
	}

	HatType type = hat->getHatType();

	switch (type) {
	case PROPELLER_HAT:
		propellerDown(player);
		break;
	case WIZARD_HAT:
		break;
	case HARD_HAT:
		break;
	case BEAR_HAT:
		break;
	}
}

void Game::propellerUp(Player* player) {
	player->getController()->getRigidBody()->applyCentralForce(btVector3(0, 10, 0));
}

void Game::propellerDown(Player* player) {
	player->getController()->getRigidBody()->applyCentralForce(btVector3(0, -10, 0));
}