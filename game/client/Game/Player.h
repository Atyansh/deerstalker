#pragma once

#include <unordered_map>

#include "SMatrixTransform.h"
#include "PlayerAnim.h"
//#include "client\Graphics\Model.h"
#include "Hat.h"
#include "util\HatType.h"
#include "util\Protos.pb.h"

class Player : public SMatrixTransform {
	private:
		SMatrixTransform *hat;
		SMatrixTransform *player;
		unordered_map<std::uint32_t, PlayerAnim*> playerModels;
		HatType currHat;
		protos::Message_GameObject_AnimationState currState;

		int lives_;
		int health_;
		int id_;
		int timer_;
		bool dead_;
		glm::vec3 position_;

		void createPlayer();

	public:
		unordered_map<std::uint32_t, Hat*> hatModels;
		bool visible_;

		Player(unordered_map<std::uint32_t, PlayerAnim*> playerModels, unordered_map<std::uint32_t, Hat*> hatModels, int id);
		~Player();

		void detachHat();
		void attachHat(HatType newHat);

		void setHealth(int health) {
			health_ = health;
		}

		int getHealth() {
			return health_;
		}

		void setLives(int lives) {
			lives_ = lives;
		}

		int getLives() {
			return lives_;
		}

		bool getVisible() {
			return visible_;
		}

		void setVisible(bool visible) {
			visible_ = visible;
		}

		glm::vec3 getPosition() {
			return position_;
		}

		void setPosition(glm::vec3 position) {
			position_ = position;
		}

		int getTimer() {
			return timer_;
		}

		void setTimer(int timer) {
			timer_ = timer;
		}

		bool getDead() {
			return dead_;
		}

		void setDead(bool dead) {
			dead_ = dead;
		}

		void changeState(protos::Message_GameObject_AnimationState newState);

		protos::Message_GameObject_AnimationState getState() {
			return currState;
		}
		
		int getID() {
			return id_;
		}
};