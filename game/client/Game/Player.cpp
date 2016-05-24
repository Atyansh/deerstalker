#include "Player.h"
#include <glm/ext.hpp>


Player::Player(unordered_map<std::uint32_t, PlayerAnim*> playerModels, unordered_map<std::uint32_t, Hat*> hatModels) : SMatrixTransform()
{
	hat = new SMatrixTransform();
	player = new SMatrixTransform();
	this->hatModels = hatModels;
	this->playerModels = playerModels;
	currHat = NO_HAT;
	currState = protos::Message_GameObject_AnimationState_STANDING;

	//add models to coresponding transform matrix
	// player->addNode(this->playerModel);
	for (auto& pair : this->playerModels) {
		player->addNode(pair.second);
	}

	for (auto& pair : this->hatModels) {
		hat->addNode(pair.second);
	}

	createPlayer();

	this->playerModels[protos::Message_GameObject_AnimationState_STANDING]->setVisible(true);
}

Player::~Player()
{
	delete hat;
	delete player;
}

void Player::createPlayer() {
	float scaleFactorPlayer = 0.075f;
	float scaleFactorHat = 2.2f;
	// add hat
	for (auto& pair : this->playerModels) {
		glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(0.f, (pair.second->getPlayerModel()->getHeight()*scaleFactorPlayer) - 0.6f, 0.f));
		hat->setMatrix(translate * glm::scale(glm::mat4(), glm::vec3(scaleFactorHat)));
		this->addNode(hat);
	}
	

	// add player
	glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(scaleFactorPlayer));
	player->setMatrix(scale);
	this->addNode(player);
}

void Player::detachHat() {
	if (currHat == NO_HAT) {
		return;
	}

	auto value = this->hatModels.find(currHat);

	if (value != this->hatModels.end()) {
		value->second->setVisible(false);
		currHat = NO_HAT;
	}
}

void Player::attachHat(HatType newHat) {
	detachHat();

	auto value = this->hatModels.find(newHat);

	if (value != this->hatModels.end()) {
		value->second->setVisible(true);
		currHat = newHat;
	}
}

void Player::changeState(protos::Message_GameObject_AnimationState newState){
	if(newState == currState){
		return;
	}

	auto valueOldState = this->playerModels.find(currState);
	auto valueNewState = this->playerModels.find(newState);

	if(valueOldState != this->playerModels.end()){
		valueOldState->second->setVisible(false);
	}

	if (valueNewState != this->playerModels.end()) {
		valueNewState->second->setVisible(true);
		currState = newState;
	}
}
