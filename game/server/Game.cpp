#include "Game.h"
#include "Serialize\BulletWorldImporter\btBulletWorldImporter.h"

using namespace std::chrono;
using namespace std::this_thread;

Game::Game() {
	idGen_ = 1;
	availableIds.emplace_back(Player::P1_ID);
	availableIds.emplace_back(Player::P2_ID);
	availableIds.emplace_back(Player::P3_ID);
	availableIds.emplace_back(Player::P4_ID);
}

Game::~Game() {
	// TODO(Atyansh): Deal with cleaning up game objects.
}

void Game::join(client_ptr client) {
	clients_.insert(client);
}

void Game::remove(client_ptr client) {
	ClientId clientId = client->getClientId();
	clients_.erase(client);
	availableIds.emplace_back(clientId);
	auto iter = playerMap_.find(clientId);
	if (iter != playerMap_.end()) {
		auto* action = iter->second->getController();
		auto* body = action->getRigidBody();
		playerSet_.erase(body);
		world_->removeAction(action);
		world_->removeRigidBody(body);
		playerMap_.erase(iter);
		delete iter->second;
	}
}

unsigned int Game::generateId() {
	return ++idGen_;
}

void Game::deliver(protos::Message msg) {
	messageQueueLock_.lock();
	messageQueue_.push_back(msg);
	messageQueueLock_.unlock();
}

int Game::size() {
	return clients_.size();
}

void Game::loadHatBodyMap() {
	hatBodyMap_[WIZARD_HAT] = wizardHatBody_;
	hatBodyMap_[HARD_HAT] = hardHatBody_;
	hatBodyMap_[BEAR_HAT] = bearHatBody_;
	hatBodyMap_[DEERSTALKER_HAT] = deerstalkerHatBody_;
	hatBodyMap_[PROPELLER_HAT] = propellerHatBody_;
}

void Game::clearAnimations() {
	for (auto client : clients_) {
		animationStateMap_[client->getClientId()] = protos::Message_GameObject_AnimationState_STANDING;
	}
}

void Game::deleteBullets() {
	milliseconds currTime = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch());

	milliseconds bulletLifespan = milliseconds(2000);

	for (auto bullet : bulletSet_) {
		if (currTime - bullet->getTimestamp() > bulletLifespan) {
			bulletRemovedSet_.emplace(bullet);
		}
	}
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

	btBulletWorldImporter* wizardHatLoader = new btBulletWorldImporter();
	wizardHatLoader->loadFile("bullet_assets\\WizardHat.bullet");
	wizardHatBody_ = playerLoader->getRigidBodyByIndex(0);

	btBulletWorldImporter* bearHatLoader = new btBulletWorldImporter();
	bearHatLoader->loadFile("bullet_assets\\BearHat.bullet");
	bearHatBody_ = playerLoader->getRigidBodyByIndex(0);

	btBulletWorldImporter* hardHatLoader = new btBulletWorldImporter();
	hardHatLoader->loadFile("bullet_assets\\HardHat.bullet");
	hardHatBody_ = playerLoader->getRigidBodyByIndex(0);

	btBulletWorldImporter* deerstalkerHatLoader = new btBulletWorldImporter();
	deerstalkerHatLoader->loadFile("bullet_assets\\DeerstalkerHat.bullet");
	deerstalkerHatBody_ = playerLoader->getRigidBodyByIndex(0);

	btBulletWorldImporter* propellerHatLoader = new btBulletWorldImporter();
	propellerHatLoader->loadFile("bullet_assets\\PropelletHat.bullet");
	propellerHatBody_ = playerLoader->getRigidBodyByIndex(0);

	loadHatBodyMap();

	btBulletWorldImporter* worldLoader = new btBulletWorldImporter(world_);
	worldLoader->loadFile("bullet_assets\\Construction_Stage_Bullet_Scaled_Separated.bullet");

	btBulletWorldImporter* billboardLoader = new btBulletWorldImporter(world_);
	billboardLoader->loadFile("bullet_assets\\BillboardPlatform.bullet");

	btBulletWorldImporter* wreckingBallBaseLoader = new btBulletWorldImporter(world_);
	wreckingBallBaseLoader->loadFile("bullet_assets\\WreckingBallBase.bullet");

	btBulletWorldImporter* wreckingBallLoader = new btBulletWorldImporter(world_);
	wreckingBallLoader->loadFile("bullet_assets\\WreckingBall.bullet");

	gravityController_ = new GravityController(this);
	world_->addAction(gravityController_);
}

void Game::loopReset() {
	gravityController_->deactivate();
	clearAnimations();

	for (auto pair : playerMap_) {
		pair.second->setVisible(true);
		pair.second->setLinearFactor(btVector3(1,1,1));
	}
}

void Game::startGameLoop() {
	// TODO(Atyansh): Make this part of config settings 
	milliseconds interval = milliseconds(1000/30);
	int frameCounter = 0;
	int maxHat = 10;
	int hatP = 0;
	while (true) {
		milliseconds stamp1 = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch());

		loopReset();

		messageQueueLock_.lock();
		while (!messageQueue_.empty()) {
			protos::Message message = messageQueue_.front();

			for (int i = 0; i < message.event_size(); i++) {
				auto event = message.event(i);
				if (event.type() == protos::Event_Type_SPAWN) {
					handleSpawnLogic(&event);
				}
				
				else if (playerMap_.count(event.clientid()) == 0) {
					//TODO WHAT CAN THE DEAD DO
					//std::cout << "WHAT CAN THE DEAD DO\n";
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
				else if (event.type() == protos::Event_Type_GRAB) {
					handleGrabLogic(&event);
				}
			}
			messageQueue_.pop_front();
		}
		messageQueueLock_.unlock();

		handleReSpawnLogic();
		world_->stepSimulation(1.f / 15.f, 10);

		deleteBullets();

		sendStateToClients();
		sendEventsToClients();

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
	bulletSet_.emplace(bull);
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
	std::cerr << "SPAWN HAPPENED" << std::endl;
	Player* player = Player::createNewPlayer(event->clientid(), playerBody_->getCollisionShape());
	playerMap_[event->clientid()] = player;
	btRigidBody* body = player->getController()->getRigidBody();
	playerSet_.emplace(body);
	world_->addRigidBody(body);
	world_->addAction(player->getController());
}

void Game::handleMoveLogic(const protos::Event* event) {
	Player* player = playerMap_[event->clientid()];

	if (animationStateMap_[event->clientid()] == protos::Message_GameObject_AnimationState_STANDING && 
		player->getController()->onGround()) {
		animationStateMap_[event->clientid()] = protos::Message_GameObject_AnimationState_RUNNING;
	}

	// TODO(Atyansh): Maybe have animation for flying?

	double x = event->cameravector(0);
	double y = event->cameravector(1);
	double z = event->cameravector(2);

	btVector3 f(-x, -y, -z);
	btVector3 b(x, y, z);
	btVector3 l(-z, y, x);
	btVector3 r(z, y, -x);

	switch (event->direction()) {
	case (protos::Event_Direction_RIGHT):
		player->getController()->playerStep(world_, r);
		break;
	case (protos::Event_Direction_LEFT):
		player->getController()->playerStep(world_, l);
		break;
	case (protos::Event_Direction_UP):
		player->getController()->getRigidBody()->applyCentralForce(btVector3(0, 10, 0));
		break;
	case (protos::Event_Direction_DOWN):
		player->getController()->getRigidBody()->applyCentralForce(btVector3(0, -10, 0));
		break;
	case (protos::Event_Direction_FORWARD):
		player->getController()->playerStep(world_, f);
		break;
	case (protos::Event_Direction_BACKWARD):
		player->getController()->playerStep(world_, b);
		break;
	case (protos::Event_Direction_FL):
		player->getController()->playerStep(world_, (f+l).normalized());
		break;
	case (protos::Event_Direction_BR):
		player->getController()->playerStep(world_, (b+r).normalized());
		break;
	case (protos::Event_Direction_BL):
		player->getController()->playerStep(world_, (b+l).normalized());
		break;
	case (protos::Event_Direction_FR):
		player->getController()->playerStep(world_, (f+r).normalized());
		break;
	}
}

void Game::handleJumpLogic(const protos::Event* event) {
	Player* player = playerMap_[event->clientid()];
	player->getController()->jump();
}

void Game::spawnNewHat() {
	HatType hatType = (HatType)((rand() % 5) + 1);
	Hat* hat = Hat::createNewHat(hatType, hatBodyMap_[hatType]->getCollisionShape());
	hatSet_.insert(hat);
	world_->addRigidBody(hat);
}

void Game::handleReSpawnLogic() {
	std::list<unsigned int> theDead;
	for (auto it = playerMap_.begin(); it != playerMap_.end(); it++) {
		if (world_->isDead(it->second)) {
			Player * currP = it->second;
			unsigned int pLives = currP->getLives() - 1;
			std::cerr << pLives << std::endl;
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
 
bool Game::withinRange(btRigidBody * body1, btRigidBody * body2) {
	//TODO MAYBE SOME BETTER SHIT
	float equipDistance = 5;
	return (equipDistance >= body1->getCenterOfMassPosition().distance(body2->getCenterOfMassPosition()));
}

void Game::handleEquipLogic(const protos::Event* event) {
	Player * player = playerMap_[event->clientid()];

	if (player->getHat() != nullptr) {
		handleDquipLogic(event);
		return;
	}

	std::cout << event->clientid() << " Attempting to equip hat\n";
	Hat* hatToRemove = nullptr;
	Hat* hatToAdd = nullptr;
	for (auto hat : hatSet_) {
		if (withinRange(player, hat)) {
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

	animationStateMap_[event->clientid()] = protos::Message_GameObject_AnimationState_PUNCHING;

	auto position = player->getController()->getRigidBody()->getCenterOfMassPosition();

	btCollisionObject* target = player->getController()->getPunchTarget();

	if (target) {
		std::cerr << "SOME TARGET" << std::endl;
		auto search = playerSet_.find(target);
		if (search != playerSet_.end()) {
			Player* punchedPlayer = (Player*)target;
			std::cerr << "PUNCH DETECTED" << std::endl;
			btVector3 localLook(0.0f, 0.0f, 1.0f);
			btTransform transform = player->getController()->getRigidBody()->getCenterOfMassTransform();
			btQuaternion rotation = transform.getRotation();
			btVector3 currentLook = quatRotate(rotation, localLook);
			punchedPlayer->applyCentralImpulse(currentLook.normalized() * 5);
			punchedPlayer->changeHealth(-5);
			// Throw Punch event to clients
			eventQueueLock_.lock();
			protos::Event event;
			event.set_type(protos::Event_Type_PLAYER_PUNCHED);
			event.set_clientid(punchedPlayer->getId());
			eventQueue_.emplace_back(event);
			eventQueueLock_.unlock();
		}
	}
}

void Game::handleGrabLogic(const protos::Event* event) {
	Player * grabber = playerMap_[event->clientid()];

	if (grabber->getGrabbedPlayer() != nullptr) {
		releaseGrab(grabber);
		return;
	}

	for (auto* body : playerSet_) {
		Player* grabbee = (Player*)body;

		if (grabber == grabbee) {
			continue;
		}

		if (!withinRange(grabber, grabbee)) {
			continue;
		}

		/*if (!grabbee->getStunned()) {
			continue;
		}*/

		grabber->setGrabbedPlayer(grabbee);
		grabbee->setMyGrabber(grabber);
		grabbee->getController()->grabOrientation(grabber);
		grabbee->setLinearFactor(btVector3(0, 0, 0));

		break;
	}
}

void Game::releaseGrab(Player* grabber) {
	Player* grabbee = grabber->getGrabbedPlayer();

	grabber->setGrabbedPlayer(nullptr);
	grabbee->setMyGrabber(nullptr);
	grabbee->getController()->straightOrientation();
	grabbee->setLinearFactor(btVector3(1, 1, 1));
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
		if (event->hold() == false) {
			handleShootLogic(event);
		}
		break;
	case HARD_HAT:
		break;
	case BEAR_HAT:
		killGravity();
		break;
	case DEERSTALKER_HAT:
		setInvisible(player);
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
		becomeBear(player);
		break;
	case DEERSTALKER_HAT:
		ramOff(event);
		break;
	}
}

void Game::becomeBear(Player* player) {
	animationStateMap_[player->getId()] = protos::Message_GameObject_AnimationState_BEAR;
	if (player->getController()->onGround()) {
		player->setLinearFactor(btVector3(0, 0, 0));
		player->setLinearVelocity(btVector3(0, 0, 0));
	}
	else {
		player->setLinearVelocity(btVector3(0, player->getLinearVelocity().getY(), 0));
		player->setLinearFactor(btVector3(0, 1, 0));
	}
}

void Game::killGravity() {
	gravityController_->activate();
}

void Game::propellerUp(Player* player) {
	player->getController()->getRigidBody()->applyCentralForce(btVector3(0, 10, 0));
}

void Game::propellerDown(Player* player) {
	player->getController()->getRigidBody()->applyCentralForce(btVector3(0, -10, 0));
}

void Game::setInvisible(Player* player) {
	player->setVisible(false);
}

void Game::ramOff(const protos::Event* event) {
	std::cerr << "ramOff" << std::endl;
	Player* player = playerMap_[event->clientid()];

	animationStateMap_[event->clientid()] = protos::Message_GameObject_AnimationState_WUSON;

	double x = event->cameravector(0);
	double y = event->cameravector(1);
	double z = event->cameravector(2);

	btVector3 f(-x, -y, -z);

	player->getController()->playerStep(world_, f);

	auto position = player->getController()->getRigidBody()->getCenterOfMassPosition();

	btCollisionObject* target = player->getController()->getRamTarget();

	if (target) {
		std::cerr << "SOME TARGET" << std::endl;
		auto search = playerSet_.find(target);
		if (search != playerSet_.end()) {
			Player* rammedPlayer = (Player*)target;
			std::cerr << "PUNCH DETECTED" << std::endl;
			btVector3 localLook(0.0f, 0.0f, 1.0f);
			btTransform transform = player->getController()->getRigidBody()->getCenterOfMassTransform();
			btQuaternion rotation = transform.getRotation();
			btVector3 ramDirection = quatRotate(rotation, localLook);
			ramDirection = ramDirection + btVector3(0, 1, 0);
			rammedPlayer->applyCentralImpulse(ramDirection.normalized() * 10);
			rammedPlayer->changeHealth(-5);
			// Throw Punch event to clients
			eventQueueLock_.lock();
			protos::Event event;
			event.set_type(protos::Event_Type_PLAYER_PUNCHED);
			event.set_clientid(rammedPlayer->getId());
			eventQueue_.emplace_back(event);
			eventQueueLock_.unlock();
		}
	}
}

void Game::sendStateToClients() {
	protos::Message message;

	for (auto& pair : playerMap_) {
		Player* player = pair.second;

		btTransform transform;
		player->getController()->getRigidBody()->getMotionState()->getWorldTransform(transform);
		btScalar glm[16] = {};

		transform.getOpenGLMatrix(glm);

		auto* gameObject = message.add_gameobject();

		Hat* hat = player->getHat();

		gameObject->set_hattype(player->getHatType());
		gameObject->set_type(protos::Message_GameObject_Type_PLAYER);
		gameObject->set_health(player->getHealth());
		gameObject->set_animationstate(animationStateMap_[player->getId()]);
		gameObject->set_visible(player->getVisible());
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

	for (auto* bullet : bulletRemovedSet_) {
		bulletSet_.erase(bullet);
		auto* event = message.add_event();
		event->set_bulletid(bullet->getId());
		event->set_type(protos::Event_Type_DELETE_BULLET);
		world_->removeRigidBody(bullet);
		delete bullet;
	}

	bulletRemovedSet_.clear();

	for (auto* bullet : bulletSet_) {
		btTransform transform;
		bullet->getMotionState()->getWorldTransform(transform);

		btScalar glm[16] = {};

		transform.getOpenGLMatrix(glm);

		auto* gameObject = message.add_gameobject();
		gameObject->set_type(protos::Message_GameObject_Type_BULLET);
		gameObject->set_id(bullet->getId());
		for (auto v : glm) {
			gameObject->add_matrix(v);
		}
	}

	for (auto client : clients_) {
		client->deliver(message);
	}
}

void Game::sendEventsToClients() {
	protos::Message message;

	eventQueueLock_.lock();
	for (auto& event : eventQueue_) {
		auto* e = message.add_event();
		e->MergeFrom(event);
	}
	eventQueue_.clear();
	eventQueueLock_.unlock();

	if (message.event_size()) {
		for (auto client : clients_) {
			client->deliver(message);
		}
	}
}