#pragma once
#include <iostream>

#include <chrono>
#include <thread>
#include <deque>

#include "Bullet.h"
#include "Client.h"
#include "World.h"
#include "Player.h"
#include "Hat.h"
#include <unordered_set>
#include <mutex>

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



private:
	void handleSpawnLogic(const protos::Event* event);
	void handleMoveLogic(const protos::Event* event);
	void handleJumpLogic(const protos::Event* event);
	void handleEquipLogic(const protos::Event* event);
	void handleShootLogic(const protos::Event* event);
	void handleDquipLogic(const protos::Event* event);
	void handlePrimaryHatLogic(const protos::Event* event);
	void handleSecondaryHatLogic(const protos::Event* event);
	void handlePunchLogic(const protos::Event* event);


	void sendStateToClients();

	void spawnNewHat();

	void propellerUp(Player* player);
	void propellerDown(Player* player);

	std::set<client_ptr> clients_;
	World* world_;

	void handleReSpawnLogic();
	bool canEquip(Player * playa, Hat * hata);

	unsigned int generateId();
	unsigned int idGen_;
	btCollisionObject* playerBody_;
	btCollisionObject* mangoBody_;

	std::unordered_map<unsigned int, Bullet*> shots_;
	std::deque<protos::Message> messageQueue_;
	std::mutex queueLock_;
	std::unordered_map<ClientId, Player*> playerMap_;
	std::unordered_set<btCollisionObject*> playerSet_;
	std::mutex playerMapLock_;
	std::unordered_set<Hat*> hatSet_;
	std::unordered_set<Hat*> hatRemovedSet_;
};