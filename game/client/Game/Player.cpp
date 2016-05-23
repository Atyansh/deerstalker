#include "Player.h"
#include <glm/ext.hpp>


Player::Player(unordered_map<std::uint32_t, PlayerAnim*> playerModels, unordered_map<std::uint32_t, Hat*> hatModels) : SMatrixTransform()
{
	hat = new SMatrixTransform();
	player = new SMatrixTransform();
	this->hatModels = hatModels;
	this->playerModels = playerModels;
	currHat = _none;
	currState = _standing;

	//add models to coresponding transform matrix
	// player->addNode(this->playerModel);
	for (auto& pair : this->playerModels) {
		player->addNode(pair.second);
	}

	for (auto& pair : this->hatModels) {
		hat->addNode(pair.second);
	}

	createPlayer();

	this->playerModels[_standing]->setVisible(true);
}

Player::~Player()
{
	delete hat;
	delete player;
}

void Player::createPlayer() {
	float scaleFactor = 0.075f;
	// add hat
	for (auto& pair : this->playerModels) {
		glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(0.f, (pair.second->getPlayerModel()->getHeight()*scaleFactor) - 0.5f, 0.f));
		hat->setMatrix(translate);
		this->addNode(hat);
	}
	

	// add player
	glm::mat4 scale = glm::scale(glm::mat4(1.f), glm::vec3(scaleFactor));
	player->setMatrix(scale);
	this->addNode(player);
}

void Player::detachHat() {
	if (currHat == _none) {
		return;
	}

	auto value = this->hatModels.find(currHat);

	if (value != this->hatModels.end()) {
		value->second->setVisible(false);
		currHat = _none;
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

void Player::changeState(PlayerState newState){
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
