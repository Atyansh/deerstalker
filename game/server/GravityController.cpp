#include "GravityController.h"

GravityController::GravityController(Game* game) : btActionInterface() {
	game_ = game;
}

GravityController::~GravityController() {

}

void GravityController::updateAction(btCollisionWorld *collisionWorld, btScalar deltaTimeStep) {
	if (active) {
		for (auto* it : game_->playerSet_) {
			Player* player = (Player*)it;
			if (player->getHatType() != BEAR_HAT) {
				player->applyCentralImpulse(btVector3(0, 10, 0) * deltaTimeStep);
			}
		}
	}
}