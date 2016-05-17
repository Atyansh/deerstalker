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
	messageQueue_.push_back(msg);
}

int Game::size() {
	return clients_.size();
}

void Game::initialize() {
	world_ = World::loadNewWorld("");
	
	btBulletWorldImporter* fileLoader = new btBulletWorldImporter();

	fileLoader->loadFile("bullet_assets\\Mango.bullet");
	body_ = fileLoader->getRigidBodyByIndex(0);

	std::cerr << "Num Rigid Bodies: " << fileLoader->getNumCollisionShapes() << std::endl;
	
}

void Game::startGameLoop() {
	// TODO(Atyansh): Make this part of config settings 
	milliseconds interval = milliseconds(33);
	int frameCounter = 0;
	int maxHat = 10;
	int hatP = 0;
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
				
				else if (playerMap_.count(event.clientid()) == 0) {
					//TODO WHAT CAN THE DEAD DO
					std::cout << "WHAT CAN THE DEAD DO\n";
					continue;
				}
				else if (event.type() == protos::Event_Type_MOVE) {
					handleMoveLogic(event);
				}
				else if (event.type() == protos::Event_Type_JUMP) {
					handleJumpLogic(event);
				}
				else if (event.type() == protos::Event_Type_EQUIP) {
					handleEquipLogic(event);
				}
				else if (event.type() == protos::Event_Type_DQUIP) {
					handleDquipLogic(event);
				}
				else if (event.type() == protos::Event_Type_SHOOT) {
					handleShootLogic(event);
				}
			
			}
			
		}	
		
		handleReSpawnLogic();
		sendStateToClients();

		milliseconds stamp2 = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch());

		sleep_for(interval - (stamp2-stamp1));

		if (frameCounter > 300 && maxHat>hatP) {
			spawnNewHat();
			hatP++;
			frameCounter = 0;
		}
		else {
			frameCounter++;
		}
	}
}

void Game::handleShootLogic(protos::Event& event) {
	std::cerr << "SHOOT HAPPENED" << std::endl;
	Player* player = playerMap_[event.clientid()];
	Bullet * bull = Bullet::createNewBullet(generateId(), body_->getCollisionShape(), player->getId());
	player->setProjectile(bull,bull->getVelocity());
	shots_[bull->getId()] = bull;
	world_->addRigidBody(bull);
}

void Game::handleDquipLogic(protos::Event& event) {
	Player* player = playerMap_[event.clientid()];
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
void Game::handleSpawnLogic(protos::Event& event) {
	std::lock_guard<std::mutex> lock(playerMapLock_);
	std::cerr << "SPAWN HAPPENED" << std::endl;
	Player* player = Player::createNewPlayer(event.clientid(), body_->getCollisionShape());
	playerMap_[event.clientid()] = player;
	world_->addRigidBody(player);
}

void Game::handleMoveLogic(protos::Event& event) {
	Player* player = playerMap_[event.clientid()];

	switch (event.direction()) {
	case (protos::Event_Direction_RIGHT) :
		std::cerr << "MOVE RIGHT" << std::endl;
		player->applyCentralForce(btVector3(10, 0, 0));
		break;
	case (protos::Event_Direction_LEFT) :
		std::cerr << "MOVE LEFT" << std::endl;
		player->applyCentralForce(btVector3(-10, 0, 0));
		break;
	case (protos::Event_Direction_UP) :
		std::cerr << "MOVE UP" << std::endl;
		player->applyCentralForce(btVector3(0, 10, 0));
		break;
	case (protos::Event_Direction_DOWN) :
		std::cerr << "MOVE DOWN" << std::endl;
		player->applyCentralForce(btVector3(0, -10, 0));
		break;
	case (protos::Event_Direction_FORWARD) :
		std::cerr << "MOVE FORWARD" << std::endl;
		player->applyCentralForce(btVector3(0, 0, -10));
		break;
	case (protos::Event_Direction_BACKWARD) :
		std::cerr << "MOVE BACKWARD" << std::endl;
		player->applyCentralForce(btVector3(0, 0, 10));
		break;
	}
}

void Game::handleJumpLogic(protos::Event& event) {
	Player* player = playerMap_[event.clientid()];

	player->applyCentralImpulse(btVector3(0, 1, 0));
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
	Hat* hat = Hat::createNewHat(Hat::WIZARD_HAT);
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
				world_->removeRigidBody(currP);
			}
			
		}

		for (auto it = theDead.begin(); it != theDead.end(); it++) {
			playerMap_.erase(*it);
		}
	}
}
 
bool Game::canEquip(Player * playa, Hat * hata) {
	//TODO MAYBE SOME BETTER SHIT
	float equipDistance = 5;
	//std::cerr << "DISTANCE " << playa->getCenterOfMassPosition().distance(hata->getCenterOfMassPosition()) << std::endl;
	//std::cerr << "WHY " << (equipDistance >= playa->getCenterOfMassPosition().distance(hata->getCenterOfMassPosition())) << std::endl;
	return equipDistance>=playa->getCenterOfMassPosition().distance(hata->getCenterOfMassPosition());
}


void Game::handleEquipLogic(protos::Event & event) {
	Player * subP = playerMap_[event.clientid()];
	std::cout << event.clientid() << " Attempting to equip hat\n";
	Hat* hatToRemove = nullptr;
	Hat* hatToAdd = nullptr;
	for (auto hats = hatSet_.begin(); hats != hatSet_.end(); hats++) {
		if (canEquip(subP, *hats)) {
			std::cerr << "Equip success\n";
			Hat * oldHat  = subP->setHat(*hats);
			world_->removeRigidBody(*hats);
			hatToRemove = *hats;
			if (oldHat != nullptr) {
				btTransform trans;
				trans.setIdentity();
				trans.setOrigin((*hats)->getCenterOfMassPosition());
				oldHat->setLinearVelocity(btVector3(0,0,0));
				oldHat->getMotionState()->setWorldTransform(trans);
				world_->addRigidBody(oldHat);
				hatToAdd = oldHat;
			}
			break;
		}
	}

	if (hatToAdd) {
		hatToAdd->playerId_ = 0;
		hatSet_.emplace(hatToAdd);
	}
	if (hatToRemove) {
		hatRemovedSet_.emplace(hatToRemove);
		hatToRemove->playerId_ = event.clientid();
		hatSet_.erase(hatToRemove);
	}
}