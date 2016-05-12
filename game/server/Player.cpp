#include "Player.h"

Player::Player() : controller_(new DynamicCharacterController()) {
}

Player::~Player() {
}

DynamicCharacterController* Player::getController() {
	return controller_;
}