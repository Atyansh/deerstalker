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

	void handleSpawnLogic(protos::Event& event);
	void handleMoveLogic(protos::Event& event);
	void handleJumpLogic(protos::Event& event);
	void handleEquipLogic(protos::Event& event);
	void handleShootLogic(protos::Event& event);
	void handleDquipLogic(protos::Event& event);
	void sendStateToClients();
	void spawnNewHat();


private:
	std::set<client_ptr> clients_;
	World* world_;

	void handleReSpawnLogic();
	bool canEquip(Player * playa, Hat * hata);

	unsigned int generateId();
	unsigned int idGen_;
	btCollisionObject* body_;
	std::unordered_map<unsigned int, Bullet*> shots_;
	std::deque<protos::Message> messageQueue_;
	std::unordered_map<ClientId, Player*> playerMap_;
	std::mutex playerMapLock_;
	std::unordered_set<Hat*> hatSet_;
	std::unordered_set<Hat*> hatRemovedSet_;
};