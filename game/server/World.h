#pragma once

#include <unordered_map>
#include "Client.h"
#include "Player.h"

class World {
public:
	World();

private:
	std::unordered_map<ClientId, Player*> playerMap_;
};