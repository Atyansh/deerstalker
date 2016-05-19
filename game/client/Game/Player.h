#pragma once

#include <unordered_map>

#include "client\Graphics\SMatrixTransform.h"
#include "PlayerModel.h"
#include "Hat.h"

class Player : public SMatrixTransform {
	private:
		SMatrixTransform *hat;
		SMatrixTransform *player;
		unordered_map<std::uint32_t, Hat*> hatModels;
		PlayerModel *playerModel; // WILL change for animation
		HatType currHat;

		void createPlayer();

	public:
		Player(PlayerModel *playerModel, unordered_map<std::uint32_t, Hat*> hatModels);
		~Player();

		void detachHat();
		void attachHat(HatType newHat);
		


};