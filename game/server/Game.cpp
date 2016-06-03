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
		world_->removeAction(action);
		world_->removeRigidBody(body);
		playerSet_.erase(body);
		playerMap_.erase(iter);
		deletedPlayers_[clientId] = iter->second;
		//delete iter->second;
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

void Game::handleGameWinLogic() {
	gameWin_ = true;
	winTimestamp_ = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch());

	eventQueueLock_.lock();
	protos::Event event;
	event.set_type(protos::Event_Type_PLAYER_WIN);
	eventQueue_.emplace_back(event);
	eventQueueLock_.unlock();
}

void Game::handleGameOverLogic() {
	milliseconds currTime = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch());

	milliseconds winSpan = milliseconds(10000);

	if (currTime - winTimestamp_ < winSpan) {
		return;
	}

	for (Player* player : deadPlayers_) {
		world_->addRigidBody(player);
		world_->addAction(player->getController());
	}

	deadPlayers_.clear();

	gameReset();
	gameWin_ = false;
	gameOn_ = false;
	eventQueueLock_.lock();
	protos::Event event;
	event.set_type(protos::Event_Type_GAME_OVER);
	eventQueue_.emplace_back(event);
	eventQueueLock_.unlock();
}

void Game::gameReset() {
	for (auto* hat : hatSet_) {
		world_->removeRigidBody(hat);
		eventQueueLock_.lock();
		protos::Event event;
		event.set_type(protos::Event_Type_EQUIP);
		event.set_hatid(hat->getHatId());
		eventQueue_.emplace_back(event);
		eventQueueLock_.unlock();
		delete hat;
	}
	hatSet_.clear();

	for (auto* body : playerSet_) {
		Player * player = (Player*)body;
		if (player->getHat()) {
			handleDquipLogic(player);
		}
		player->reset();
	}

	for (auto* hat : hatSet_) {
		world_->removeRigidBody(hat);
		delete hat;
	}
	hatSet_.clear();
}

void Game::deleteBullets() {
	milliseconds currTime = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch());

	milliseconds bulletLifespan = milliseconds(2000);

	for (auto* bullet : bulletSet_) {
		if (currTime - bullet->getTimestamp() > bulletLifespan) {
			bulletRemovedSet_.emplace(bullet);
		}
	}
}

void Game::deleteHats() {
	milliseconds currTime = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch());

	for (auto* hat : hatSet_) {
		if (currTime - hat->getTimestamp() > Hat::HAT_LIFESPAN) {
			hatRemovedSet_.emplace(hat);
			world_->removeRigidBody(hat);
		}
	}

	for (auto* body : playerSet_) {
		Player* player = (Player*)body;
		Hat* hat = player->getHat();

		if (hat) {
			if (currTime - hat->getTimestamp() > Hat::HAT_LIFESPAN) {
				protos::Event event;
				event.set_clientid(player->getId());
				handleDquipLogic(player);
			}
		}
	}
}

void Game::detectStun() {
	milliseconds currTime = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch());

	for (auto* body : playerSet_) {
		Player* player = (Player*)body;
		if (!player->getStunned() && player->getHealth() == 0) {
			if (player->getHat()) {
				handleDquipLogic(player);
			}
			player->setStunned(true);
			player->setStunTimestamp(currTime);
		}
	}
}

void Game::revivePlayers() {
	milliseconds currTime = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch());

	milliseconds reviveSpan = milliseconds(10000);

	for (auto* body : playerSet_) {
		Player* player = (Player*)body;
		if (player->getStunned()) {
			if (currTime - player->getStunTimestamp() > reviveSpan) {
				if (player->getHealth() < 50) {
					player->setHealth(50);
				}
				player->setStunned(false);

				Player* grabber = player->getMyGrabber();

				if (grabber) {
					releaseGrab(grabber);
				}
			}
		}
	}
}

void Game::resurrectPlayers() {
	milliseconds currTime = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch());

	milliseconds resurrectSpan = milliseconds(4000);

	for (auto* body : playerSet_) {
		Player* player = (Player*)body;

		if (player->getDead() && player->getLives() > 0 &&
			(currTime - player->getDeadTimestamp() > resurrectSpan)) {
			player->setDead(false);
			player->setHealth(100);
			player->setStunned(false);
			world_->addRigidBody(player);
			world_->addAction(player->getController());
			world_->spawnPlayer(player);
		}
	}
}

void Game::initialize() {
	world_ = World::createNewWorld();
	world_->setGravity(btVector3(0, -10, 0));

	btCollisionShape* skyShape = new btStaticPlaneShape(btVector3(0,-1,0), 5);
	btTransform skyTransform;
	skyTransform.setIdentity();
	skyTransform.setOrigin(btVector3(0.0, 1005, 0.0));
	btDefaultMotionState* myMotionState = new btDefaultMotionState(skyTransform);
	btRigidBody::btRigidBodyConstructionInfo skyInfo(0, myMotionState, skyShape);
	btRigidBody* sky = new btRigidBody(skyInfo);
	world_->addRigidBody(sky);

	btCollisionShape* wallShape1 = new btStaticPlaneShape(btVector3(1, 0, 0), 5);
	btTransform wallTransform1;
	wallTransform1.setIdentity();
	wallTransform1.setOrigin(btVector3(-1000, 0.0, 0.0));
	btDefaultMotionState* wallState1 = new btDefaultMotionState(wallTransform1);
	btRigidBody::btRigidBodyConstructionInfo wallInfo1(0, wallState1, wallShape1);
	btRigidBody* wall1 = new btRigidBody(wallInfo1);
	world_->addRigidBody(wall1);

	btCollisionShape* wallShape2 = new btStaticPlaneShape(btVector3(-1, 0, 0), 5);
	btTransform wallTransform2;
	wallTransform2.setIdentity();
	wallTransform2.setOrigin(btVector3(1000, 0.0, 0.0));
	btDefaultMotionState* wallState2 = new btDefaultMotionState(wallTransform2);
	btRigidBody::btRigidBodyConstructionInfo wallInfo2(0, wallState2, wallShape2);
	btRigidBody* wall2 = new btRigidBody(wallInfo2);
	world_->addRigidBody(wall2);

	btCollisionShape* wallShape3 = new btStaticPlaneShape(btVector3(0, 0, 1), 5);
	btTransform wallTransform3;
	wallTransform3.setIdentity();
	wallTransform3.setOrigin(btVector3(0.0, 0.0, -1000));
	btDefaultMotionState* wallState3 = new btDefaultMotionState(wallTransform3);
	btRigidBody::btRigidBodyConstructionInfo wallInfo3(0, wallState3, wallShape3);
	btRigidBody* wall3 = new btRigidBody(wallInfo3);
	world_->addRigidBody(wall3);

	btCollisionShape* wallShape4 = new btStaticPlaneShape(btVector3(0, 0, -1), 5);
	btTransform wallTransform4;
	wallTransform4.setIdentity();
	wallTransform4.setOrigin(btVector3(0.0, 0.0, 1000));
	btDefaultMotionState* wallState4 = new btDefaultMotionState(wallTransform4);
	btRigidBody::btRigidBodyConstructionInfo wallInfo4(0, wallState4, wallShape4);
	btRigidBody* wall4 = new btRigidBody(wallInfo4);
	world_->addRigidBody(wall4);

	btBulletWorldImporter* playerLoader = new btBulletWorldImporter();
	playerLoader->loadFile("bullet_assets\\Player.bullet");
	playerBody_ = playerLoader->getRigidBodyByIndex(0);

	playerBody_->getCollisionShape()->setLocalScaling(btVector3(1.5, 1.5, 1.5));

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
	worldLoader->loadFile("bullet_assets\\Construction_Stage_3.bullet");

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
		pair.second->setBearMode(false);
	}
}

void Game::startGameLoop() {
	// TODO(Atyansh): Make this part of config settings 
	milliseconds interval = milliseconds(1000/30);
	int frameCounter = 0;
	while (true) {
		milliseconds stamp1 = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch());

		if (gameOn_ && !gameWin_ && deadPlayers_.size() >= MIN_PLAYERS - 1) {
			handleGameWinLogic();
		}

		if (gameWin_) {
			handleGameOverLogic();
		}

		loopReset();

		messageQueueLock_.lock();
		while (!messageQueue_.empty()) {
		//std::cerr << "in Message Queue" << std::endl;
			protos::Message message = messageQueue_.front();

			for (int i = 0; i < message.event_size(); i++) {
				auto event = message.event(i);
				if (event.type() == protos::Event_Type_SPAWN) {
					handleSpawnLogic(&event);
					eventQueueLock_.lock();
					for (auto item : playerMap_) {
						if (item.second->getReady()) {
							protos::Event e;
							e.set_type(protos::Event_Type_READY);
							e.set_clientid(item.first);
							eventQueue_.emplace_back(e);
						}
					}
					eventQueueLock_.unlock();
				}
				else if (playerMap_[event.clientid()]->getDead()) {
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
				else if (event.type() == protos::Event_Type_READY) {
					//std::cerr << "READY RECEIVED FROM CLIENT" << std::endl;
					playerMap_[event.clientid()]->setReady(true);
					eventQueueLock_.lock();
					protos::Event e;
					e.set_type(protos::Event_Type_READY);
					e.set_clientid(event.clientid());
					eventQueue_.emplace_back(e);
					eventQueueLock_.unlock();
				}
			}
			messageQueue_.pop_front();
		}
		messageQueueLock_.unlock();

		if (!getGameOn() && allReady()) {
			setGameOn(true);
			gameReset();
			//std::cerr << "ALL READY" << std::endl;
			eventQueueLock_.lock();
			protos::Event e;
			e.set_type(protos::Event_Type_START_GAME);
			eventQueue_.emplace_back(e);
			eventQueueLock_.unlock();
		}

		handleReSpawnLogic();
		world_->stepSimulation(1.f / 15.f, 10);

		deleteBullets();
		deleteHats();

		detectStun();
		revivePlayers();
		resurrectPlayers();

		if (gravityController_->getActive()) {
			eventQueueLock_.lock();
			protos::Event event;
			event.set_type(protos::Event_Type_GRAVITY_MUSIC);
			eventQueue_.emplace_back(event);
			eventQueueLock_.unlock();
		}
		else {
			eventQueueLock_.lock();
			protos::Event event;
			event.set_type(protos::Event_Type_GAME_MUSIC);
			eventQueue_.emplace_back(event);
			eventQueueLock_.unlock();
		}

		sendStateToClients();

		milliseconds stamp2 = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch());

		sleep_for(interval - (stamp2 - stamp1));

		if (frameCounter > 300) {
			spawnNewHat();
			frameCounter = 0;
		}
		else {
			frameCounter++;
		}
	}
}

void Game::handleShootLogic(const protos::Event* event) {
	Player* player = playerMap_[event->clientid()];
	animationStateMap_[player->getId()] = protos::Message_GameObject_AnimationState_WAND;
	if (event->hold() == false) {
		Bullet * bull = Bullet::createNewBullet(generateId(), mangoBody_->getCollisionShape(), player->getId());
		player->setProjectile(bull, bull->getVelocity());
		bulletSet_.emplace(bull);
		world_->addRigidBody(bull);
	}
}

void Game::handleDquipLogic(Player* player) {
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
	//std::cerr << "SPAWN HAPPENED" << std::endl;

	Player* player;

	if (deletedPlayers_.find(event->clientid()) != deletedPlayers_.end()) {
		player = deletedPlayers_[event->clientid()];
		deletedPlayers_.erase(event->clientid());
		player->reset();
	}
	else {
		player = Player::createNewPlayer(event->clientid(), playerBody_->getCollisionShape());
	}

	playerMap_[event->clientid()] = player;;
	playerSet_.emplace(player);
	world_->addRigidBody(player);
	world_->addAction(player->getController());
}

void Game::handleMoveLogic(const protos::Event* event) {
	Player* player = playerMap_[event->clientid()];

	if (animationStateMap_[event->clientid()] == protos::Message_GameObject_AnimationState_STANDING && 
		player->getController()->onGround()) {
		animationStateMap_[event->clientid()] = protos::Message_GameObject_AnimationState_RUNNING;
	}

	if (player->getGrabbedPlayer()) {
		if (player->getController()->onGround()) {
			animationStateMap_[event->clientid()] = protos::Message_GameObject_AnimationState_GRAB_WALK;
		}
		else {
			animationStateMap_[event->clientid()] = protos::Message_GameObject_AnimationState_GRAB;
		}
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
	if (player->getController()->onGround()) {
		player->getController()->jump();

		eventQueueLock_.lock();
		protos::Event jumpEvent;
		jumpEvent.set_type(protos::Event_Type_PLAYER_JUMP);
		jumpEvent.set_clientid(event->clientid());
		eventQueue_.emplace_back(jumpEvent);
		eventQueueLock_.unlock();
	}
}

void Game::spawnNewHat() {
	HatType hatType = (HatType)((rand() % 5) + 1);
	Hat* hat = Hat::createNewHat(hatType, hatBodyMap_[hatType]->getCollisionShape());
	hatSet_.insert(hat);
	world_->addRigidBody(hat);
}

void Game::handleReSpawnLogic() {
	std::unordered_set<Player*> theDead;
	for (auto* body : playerSet_) {
		Player* player = (Player*)body;
		if (player->getDead()) {
			continue;
		}
		if (world_->isDead(player)) {
			auto lives = player->getLives();

			eventQueueLock_.lock();
			protos::Event event;
			event.set_clientid(player->getId());
			event.set_type(protos::Event_Type_PLAYER_DIED);
			eventQueue_.emplace_back(event);
			eventQueueLock_.unlock();

			Hat* hat = player->getHat();
			if (hat) {
				handleDquipLogic(player);
			}

			world_->removeRigidBody(player);

			player->setLives(lives - 1);
			player->setDeadTimestamp(duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()));
			player->setDead(true);

			//std::cerr << "Player died" << std::endl;

			if (player->getLives() == 0) {
				/*
				eventQueueLock_.lock();
				event.Clear();
				event.set_clientid(player->getId());
				event.set_type(protos::Event_Type_GAME_OVER);
				eventQueue_.emplace_back(event);
				eventQueueLock_.unlock();
				*/
				deadPlayers_.emplace(player);
			}
		}
	}
}
 
bool Game::withinRange(btRigidBody * body1, btRigidBody * body2, int range) {
	//TODO MAYBE SOME BETTER SHIT
	return (range >= body1->getCenterOfMassPosition().distance(body2->getCenterOfMassPosition()));
}

void Game::handleEquipLogic(const protos::Event* event) {
	Player * player = playerMap_[event->clientid()];

	if (player->getHat() != nullptr) {
		handleDquipLogic(player);
		return;
	}

	//std::cout << event->clientid() << " Attempting to equip hat\n";
	Hat* hatToRemove = nullptr;
	Hat* hatToAdd = nullptr;
	for (auto hat : hatSet_) {
		if (withinRange(player, hat, 7)) {
			//std::cerr << "Equip success\n";
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

	auto position = player->getCenterOfMassPosition();

	btCollisionObject* target = player->getController()->getPunchTarget();

	if (target) {
		//std::cerr << "SOME TARGET" << std::endl;
		auto search = playerSet_.find(target);
		if (search != playerSet_.end()) {
			Player* punchedPlayer = (Player*)target;
			//std::cerr << "PUNCH DETECTED" << std::endl;
			btVector3 localLook(0.0f, 0.0f, 1.0f);
			btTransform transform = player->getController()->getRigidBody()->getCenterOfMassTransform();
			btQuaternion rotation = transform.getRotation();
			btVector3 currentLook = quatRotate(rotation, localLook);
			punchedPlayer->applyCentralImpulse((currentLook.normalized() + btVector3(0, 1, 0)) * 10);
			punchedPlayer->changeHealth(-10);
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

		if (!withinRange(grabber, grabbee, 10)) {
			continue;
		}

		if (grabbee->getMyGrabber()) {
			continue;
		}

		if (!grabbee->getStunned()) {
			continue;
		}

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

	grabber->setProjectile(grabbee, 50);
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
		wrenchHit(player);
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
		heal(player);
		break;
	case HARD_HAT:
		shockwave(player);
		break;
	case BEAR_HAT:
		becomeBear(player);
		break;
	case DEERSTALKER_HAT:
		ramOff(event);
		break;
	}
}

void Game::heal(Player* player) {
	player->changeHealth(1);
}

void Game::wrenchHit(Player* player) {
	// TODO Add Wrench animation
	auto position = player->getCenterOfMassPosition();

	btCollisionObject* target = player->getController()->getWrenchTarget();

	animationStateMap_[player->getId()] = protos::Message_GameObject_AnimationState_WRENCH_SWING;

	if (target) {
		auto search = playerSet_.find(target);
		if (search != playerSet_.end()) {
			Player* wrenchedPlayer = (Player*)target;
			btVector3 localLook(0.0f, 0.0f, 1.0f);
			btTransform transform = player->getController()->getRigidBody()->getCenterOfMassTransform();
			btQuaternion rotation = transform.getRotation();
			btVector3 currentLook = quatRotate(rotation, localLook);
			wrenchedPlayer->applyCentralImpulse((currentLook.normalized() + btVector3(0, 1, 0)) * 10);
			wrenchedPlayer->changeHealth(-100);
			// Throw Punch event to clients
			eventQueueLock_.lock();
			protos::Event event;
			// TODO Change this to wrench like event
			event.set_type(protos::Event_Type_PLAYER_PUNCHED);
			event.set_clientid(wrenchedPlayer->getId());
			eventQueue_.emplace_back(event);
			eventQueueLock_.unlock();
		}
	}
}

void Game::shockwave(Player* player) {
	// TODO add shockwave animation

	if (!player->getController()->onGround()) {
		return;
	}

	btVector3 playerPos = player->getCenterOfMassPosition();
	animationStateMap_[player->getId()] = protos::Message_GameObject_AnimationState_WRENCH_SLAM;

	for (auto* body : playerSet_) {
		Player* shockedPlayer = (Player*)body;
		if (shockedPlayer == player) {
			continue;
		}
		if (withinRange(player, shockedPlayer, 30) && shockedPlayer->getController()->onGround()) {
			btVector3 shockPos = shockedPlayer->getCenterOfMassPosition();
			btVector3 direction = (shockPos - playerPos).normalized();
			shockedPlayer->applyCentralImpulse(direction * 5);
			shockedPlayer->changeHealth(-10);
		}
	}

	for (auto* hat : hatSet_) {
		if (withinRange(player, hat, 15)) {
			btVector3 shockPos = hat->getCenterOfMassPosition();
			btVector3 direction = (shockPos - playerPos).normalized();
			hat->applyCentralImpulse(direction * 5);
		}
	}
}

void Game::becomeBear(Player* player) {
	animationStateMap_[player->getId()] = protos::Message_GameObject_AnimationState_BEAR;
	player->setBearMode(true);
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
	eventQueueLock_.lock();
	animationStateMap_[player->getId()] = protos::Message_GameObject_AnimationState_FLYING;
	protos::Event propellerEvent;
	propellerEvent.set_type(protos::Event_Type_PLAYER_PROPELLER);
	propellerEvent.set_clientid(player->getId());
	eventQueue_.emplace_back(propellerEvent);
	eventQueueLock_.unlock();
}

void Game::propellerDown(Player* player) {
	player->getController()->getRigidBody()->applyCentralForce(btVector3(0, -10, 0));
	eventQueueLock_.lock();
	animationStateMap_[player->getId()] = protos::Message_GameObject_AnimationState_FLYING;
	protos::Event propellerEvent;
	propellerEvent.set_type(protos::Event_Type_PLAYER_PROPELLER);
	propellerEvent.set_clientid(player->getId());
	eventQueue_.emplace_back(propellerEvent);
	eventQueueLock_.unlock();
}

void Game::setInvisible(Player* player) {
	player->setVisible(false);
}

void Game::ramOff(const protos::Event* event) {
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
		//std::cerr << "SOME TARGET" << std::endl;
		auto search = playerSet_.find(target);
		if (search != playerSet_.end()) {
			Player* rammedPlayer = (Player*)target;
			//std::cerr << "PUNCH DETECTED" << std::endl;
			btVector3 localLook(0.0f, 0.0f, 1.0f);
			btTransform transform = player->getController()->getRigidBody()->getCenterOfMassTransform();
			btQuaternion rotation = transform.getRotation();
			btVector3 ramDirection = quatRotate(rotation, localLook);
			ramDirection = ramDirection + btVector3(0, 1, 0);
			rammedPlayer->applyCentralImpulse(ramDirection.normalized() * 20);
			rammedPlayer->changeHealth(-20);
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

		auto* gameObject = message.add_gameobject();

		btVector3 position = player->getCenterOfMassPosition();

		//std::cerr << position.getX() << " " << position.getY() << " " << position.getZ() << std::endl;

		btTransform transform;
		player->getController()->getRigidBody()->getMotionState()->getWorldTransform(transform);
		btScalar glm[16] = {};

		transform.getOpenGLMatrix(glm);

		if (player->getGrabbedPlayer()) {
			if (animationStateMap_[player->getId()] != protos::Message_GameObject_AnimationState_GRAB_WALK) {
				animationStateMap_[player->getId()] = protos::Message_GameObject_AnimationState_GRAB;
			}
		}

		if (player->getStunned()) {
			animationStateMap_[player->getId()] = protos::Message_GameObject_AnimationState_STUNNED;
		}

		if (player->getDead()) {
			animationStateMap_[player->getId()] = protos::Message_GameObject_AnimationState_DEAD;
		}

		Hat* hat = player->getHat();

		if (hat) {
			milliseconds currTime = duration_cast<milliseconds>(
				system_clock::now().time_since_epoch());

			milliseconds timer = currTime - hat->getTimestamp();

			gameObject->set_timer(timer.count());
		}
		else {
			gameObject->set_timer(0);
		}

		gameObject->set_posx(position.getX());
		gameObject->set_posy(position.getY());
		gameObject->set_posz(position.getZ());
		gameObject->set_hattype(player->getHatType());
		gameObject->set_dead(player->getDead());
		gameObject->set_type(protos::Message_GameObject_Type_PLAYER);
		gameObject->set_health(player->getHealth());
		gameObject->set_lives(player->getLives());
		gameObject->set_animationstate(animationStateMap_[player->getId()]);
		gameObject->set_visible(player->getVisible());
		gameObject->set_id(player->getId());
		for (auto v : glm) {
			gameObject->add_matrix(v);
		}
	}

	for (auto* hat : hatRemovedSet_) {
		auto* event = message.add_event();
		event->set_type(protos::Event_Type_EQUIP);
		event->set_clientid(hat->playerId_);
		event->set_hatid(hat->getHatId());
		
		if (hat->playerId_ == 0) {
			hatSet_.erase(hat);
			delete hat;
		}
	}

	hatRemovedSet_.clear();

	for (auto* hat : hatSet_) {
		btTransform transform;
		hat->getMotionState()->getWorldTransform(transform);

		btScalar glm[16] = {};

		transform.getOpenGLMatrix(glm);

		btVector3 position = hat->getCenterOfMassPosition();

		auto* gameObject = message.add_gameobject();
		gameObject->set_posx(position.getX());
		gameObject->set_posy(position.getY());
		gameObject->set_posz(position.getZ());
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

		btVector3 position = bullet->getCenterOfMassPosition();

		btScalar glm[16] = {};

		transform.getOpenGLMatrix(glm);

		auto* gameObject = message.add_gameobject();
		gameObject->set_posx(position.getX());
		gameObject->set_posy(position.getY());
		gameObject->set_posz(position.getZ());
		gameObject->set_type(protos::Message_GameObject_Type_BULLET);
		gameObject->set_id(bullet->getId());
		for (auto v : glm) {
			gameObject->add_matrix(v);
		}
	}

	sleep_for(milliseconds(1));

	eventQueueLock_.lock();
	for (auto& event : eventQueue_) {
		auto* e = message.add_event();
		e->MergeFrom(event);
	}
	eventQueue_.clear();
	eventQueueLock_.unlock();

	for (auto client : clients_) {
		client->deliver(message);
	}
}