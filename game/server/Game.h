#pragma once

#define _USE_MATH_DEFINES

#include <iostream>

#include <chrono>
#include <thread>
#include <deque>

#include "Client.h"
#include "World.h"
#include "Player.h"
#include "Bullet.h"
#include <tuple>
#include <mutex>
#include <cmath>


class Game {
public:
	Game();
	~Game();

	void join(client_ptr client);
	void remove(client_ptr client);
	void deliver(protos::Message msg);
	int size();
	int generateId();

	void initialize();
	void startGameLoop();

	void handleSpawnLogic(protos::Event& event);
	void handleMoveLogic(protos::Event& event);
	void handleJumpLogic(protos::Event& event);
	void handleShootLogic(protos::Event& event);
	void handleViewLogic(protos::Event& event);

	void sendStateToClients();
   
private:
	// TODO MOVE TO CONSTANTS LOCATION
	const btScalar ROT_DELTA = M_PI / 8;
	
	int idCounter_;
	std::set<client_ptr> clients_;
	World* world_;

	btCollisionObject* mango_body_;
	btCollisionObject* player_body_;

	std::deque<protos::Message> messageQueue_;
	std::unordered_map<ClientId, Player*> playerMap_;
	std::list<Bullet*> itemList_;
	std::mutex playerMapLock_;

	std::tuple<btVector3, btScalar> getRotation(protos::Event_Direction direction, Player * player);
};
