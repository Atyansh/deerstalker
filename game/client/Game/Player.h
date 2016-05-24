#pragma once

#include <unordered_map>

#include "SMatrixTransform.h"
#include "PlayerAnim.h"
//#include "client\Graphics\Model.h"
#include "Hat.h"
#include "util\HatType.h"

class Player : public SMatrixTransform {
	private:
		SMatrixTransform *hat;
		SMatrixTransform *player;
		unordered_map<std::uint32_t, Hat*> hatModels;
		unordered_map<std::uint32_t, PlayerAnim*> playerModels;
		HatType currHat;
		PlayerState currState;

		void createPlayer();

	public:
		Player(unordered_map<std::uint32_t, PlayerAnim*> playerModels, unordered_map<std::uint32_t, Hat*> hatModels);
		~Player();

		void detachHat();
		void attachHat(HatType newHat);
		void changeState(PlayerState newState);

};