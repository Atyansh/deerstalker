#pragma once

#include <unordered_map>

#include "SMatrixTransform.h"
#include "Model.h"
#include "Hat.h"

class Player : public SMatrixTransform {
	private:
		SMatrixTransform *hat;
		SMatrixTransform *player;
		unordered_map<std::uint32_t, Hat*> hatModels;
		Model *playerModel; // WILL change for animation
		HatType currHat;

		void createPlayer();

	public:
		Player(Model *playerModel, unordered_map<std::uint32_t, Hat*> hatModels);
		~Player();

		void detachHat();
		void attachHat(HatType newHat);
		


};