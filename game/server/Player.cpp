#include "Player.h"

Player::Player(btCollisionObject* body) : controller_(new DynamicCharacterController(body)) {
}

Player::~Player() {
}

DynamicCharacterController* Player::getController() {
	return controller_;
}