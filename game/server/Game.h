#pragma once
#include <iostream>

#include <chrono>
#include <thread>
#include <deque>
#include <unordered_set>
#include <mutex>

#include "Bullet.h"
#include "Client.h"
#include "World.h"
#include "Player.h"
#include "Hat.h"
#include "GravityController.h"


class GravityController;

class Game {
public:
	Game();
	~Game();

	void join(client_ptr client);
	void remove(client_ptr client);
	void deliver(protos::Message msg);
	int size();

	void initialize();
	void startGameLoop();

	const static uint32_t MIN_PLAYERS = 1;
	const static uint32_t MAX_PLAYERS = 4;
	std::deque<uint32_t> availableIds;

	std::unordered_set<btCollisionObject*> playerSet_;

private:
	void loadHatBodyMap();

	void clearAnimations();

	bool allReady() {
		if (size() < MIN_PLAYERS) {
			return false;
		}

		for (auto* body : playerSet_) {
			Player* player = (Player*)body;
			if (!player->getReady()) {
				return false;
			}
			return true;
		}
	}

	void deleteBullets();
	void deleteHats();

	void detectStun();
	void revivePlayers();
	void resurrectPlayers();

	void loopReset();

	void handleSpawnLogic(const protos::Event* event);
	void handleMoveLogic(const protos::Event* event);
	void handleJumpLogic(const protos::Event* event);
	void handleEquipLogic(const protos::Event* event);
	void handleShootLogic(const protos::Event* event);
	void handleDquipLogic(Player* player);
	void handlePrimaryHatLogic(const protos::Event* event);
	void handleSecondaryHatLogic(const protos::Event* event);
	void handlePunchLogic(const protos::Event* event);
	void handleGrabLogic(const protos::Event* event);

	bool withinRange(btRigidBody * body1, btRigidBody * body2, int range);

	void releaseGrab(Player* player);

	void sendStateToClients();

	void spawnNewHat();

	void propellerUp(Player* player);
	void propellerDown(Player* player);

	void setInvisible(Player* player);
	void ramOff(const protos::Event* event);

	void becomeBear(Player* player);
	void killGravity();

	void wrenchHit(Player* player);
	void shockwave(Player* player);

	std::set<client_ptr> clients_;
	World* world_;

	void handleReSpawnLogic();

	unsigned int generateId();
	unsigned int idGen_;

	btCollisionObject* playerBody_;
	btCollisionObject* mangoBody_;
	btCollisionObject* wizardHatBody_;
	btCollisionObject* bearHatBody_;
	btCollisionObject* hardHatBody_;
	btCollisionObject* deerstalkerHatBody_;
	btCollisionObject* propellerHatBody_;


	std::deque<protos::Message> messageQueue_;
	std::mutex messageQueueLock_;

	std::deque<protos::Event> eventQueue_;
	std::mutex eventQueueLock_;

	std::unordered_set<Bullet*> bulletSet_;
	std::unordered_set<Bullet*> bulletRemovedSet_;

	std::unordered_map<ClientId, Player*> playerMap_;
	std::unordered_set<Player*> deadPlayers_;

	std::unordered_set<Hat*> hatSet_;
	std::unordered_set<Hat*> hatRemovedSet_;

	std::unordered_map<HatType, btCollisionObject*> hatBodyMap_;
	std::unordered_map<ClientId, protos::Message_GameObject_AnimationState> animationStateMap_;

	GravityController* gravityController_;
};