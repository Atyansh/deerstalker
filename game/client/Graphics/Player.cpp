#include "Player.h"
#include <glm/ext.hpp>


Player::Player(Model *playerModel, unordered_map<std::uint32_t, Hat*> hatModels) : SMatrixTransform()
{
	hat = new SMatrixTransform();
	player = new SMatrixTransform();
	this->hatModels = hatModels;
	this->playerModel = playerModel;
	currHat = _none;

	//add models to coresponding transform matrix
	player->addNode(this->playerModel);
	for (auto& pair : this->hatModels) {
		hat->addNode(pair.second);
	}

	createPlayer();
}

Player::~Player()
{
	delete hat;
	delete player;
}

void Player::createPlayer() {
	// add hat
	glm::mat4 translate = glm::translate(glm::mat4(1.f), glm::vec3(0.f,this->playerModel->getHeight()-0.5f, 0.f));
	hat->setMatrix(translate);
	this->addNode(hat);

	// add player
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