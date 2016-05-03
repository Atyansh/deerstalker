#include "Player.h"

char* Player::modelPath = "Graphics/Assets/OBJ/Player/Player.obj";

Player::Player() {
}

Player::~Player() {
}

Player* Player::createNewPlayer() {
	Model* model = new Model(modelPath, Globals::lightShader);

	Player* player = new Player();
	player->setMatrix(glm::mat4());
	player->addNode(model);

	return player;
}