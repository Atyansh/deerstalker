#include "MessageHandler.h"


std::deque<protos::Message> MessageHandler::lobbyQueue = std::deque<protos::Message>();
std::mutex MessageHandler::lobbyLock;

std::deque<protos::Message> MessageHandler::startQueue = std::deque<protos::Message>();
std::mutex MessageHandler::startLock;

std::deque<protos::Message> MessageHandler::gameQueue = std::deque<protos::Message>();
std::mutex MessageHandler::gameLock;

std::deque<protos::Message> MessageHandler::endGameQueue = std::deque<protos::Message>();
std::mutex MessageHandler::endGameLock;

void MessageHandler::handleLobbyMessages() {
}

void MessageHandler::handleStartMessages() {
}

void MessageHandler::handleGameMessages() {
	gameLock.lock();
	while (!gameQueue.empty()) {
		protos::Message& message = gameQueue.front();

		for (int i = 0; i < message.event_size(); i++) {
			auto& event = message.event(i);

			if (event.type() == protos::Event_Type_EQUIP) {
				auto* hat = Globals::gameObjects.hatMap[event.hatid()];
				Globals::gameObjects.hatMap.erase(event.hatid());
				Globals::soundEngine.wearHat(event.clientid());
				//delete hat;
			}
			else if (event.type() == protos::Event_Type_DELETE_BULLET) {
				auto* bullet = Globals::gameObjects.bulletMap[event.bulletid()];
				Globals::gameObjects.bulletMap.erase(event.bulletid());
				//delete bullet;
			}
			else if (event.type() == protos::Event_Type_PLAYER_JUMP) {
				Globals::soundEngine.jump(event.clientid());
			}
			else if (event.type() == protos::Event_Type_PLAYER_PROPELLER) {
				Globals::soundEngine.propeller(event.clientid());
			}
			else if (event.type() == protos::Event_Type_PLAYER_PUNCHED) {
				Globals::soundEngine.punch(event.clientid());
			}
			else if (event.type() == protos::Event_Type_GRAVITY_MUSIC) {
				Globals::soundEngine.playGravityMusic();
			}
			else if (event.type() == protos::Event_Type_GAME_MUSIC) {
				Globals::soundEngine.playGameMusic();
			}
			else if (event.type() == protos::Event_Type_PLAYER_DIED) {
				//Globals::gameObjects.playerMap.erase(event.clientid());
				std::cerr << "PLAYER IS DEAD" << std::endl;
			}
		}

		for (int i = 0; i < message.gameobject_size(); i++) {
			Models model;
			auto& gameObject = message.gameobject(i);
			int id = gameObject.id();

			auto* map = &Globals::gameObjects.playerMap;

			if (gameObject.type() == protos::Message_GameObject_Type_PLAYER) {
				map = &Globals::gameObjects.playerMap;
				model = _Player;
			}
			else if (gameObject.type() == protos::Message_GameObject_Type_HAT) {
				map = &Globals::gameObjects.hatMap;
				model = Globals::hatModelsMap[(HatType)gameObject.hattype()]; // change
			}
			else if (gameObject.type() == protos::Message_GameObject_Type_BULLET) {
				map = &Globals::gameObjects.bulletMap;
				model = _Mango;
			}

			float matrix[16];
			for (int j = 0; j < gameObject.matrix_size(); j++) {
				matrix[j] = gameObject.matrix(j);
			}

			if ((*map).find(id) == (*map).end()) {
				(*map)[id] = Window::createGameObj(model, Globals::gameObjects.modelMap[model], id);
				if (gameObject.type() == protos::Message_GameObject_Type_PLAYER) {
					if (id == Globals::ID) {
						Player* player = dynamic_cast<Player*>(Globals::gameObjects.playerMap[Globals::ID]);
						Globals::soundEngine.initializePlayer(player);
					}
				}
				if (gameObject.type() == protos::Message_GameObject_Type_BULLET) {
					Globals::soundEngine.mangoShot(gameObject.posx(), gameObject.posy(), gameObject.posz());
				}
				if (gameObject.type() == protos::Message_GameObject_Type_HAT) {
					Globals::soundEngine.hatSpawn(gameObject.posx(), gameObject.posy(), gameObject.posz());
				}
			}

			auto& entity = *(*map)[id];

			glm::mat4 mat = glm::make_mat4(matrix);

			entity.setMatrix(mat);
			if (gameObject.type() == protos::Message_GameObject_Type_PLAYER) {
				Player* player = dynamic_cast<Player*>(&entity);

				glm::vec3 position = glm::vec3(gameObject.posx(), gameObject.posy(), gameObject.posz());

				player->setDead(gameObject.dead());
				player->setPosition(position);
				player->setTimer(gameObject.timer());
				player->setLives(gameObject.lives());
				player->setHealth(gameObject.health());

				if (gameObject.animationstate() == protos::Message_GameObject_AnimationState_BEAR) {
					auto scale = glm::vec3(1) / 0.075;
					glm::mat4 scaleMat = glm::scale(mat, scale);
					entity.setMatrix(scaleMat);
				}
				else if (gameObject.animationstate() == protos::Message_GameObject_AnimationState_WUSON) {
					Globals::soundEngine.wuson(id);
					auto scale = glm::vec3(1) / 0.075 * 8;
					glm::mat4 scaleMat = glm::scale(mat, scale);
					entity.setMatrix(scaleMat);
				}

				if (Globals::ID != id) {
					player->setVisible(gameObject.visible());
				}
				else {
					player->setVisible(true);
				}

				if (gameObject.id() == Globals::ID) { // follow camera based player
					Globals::cam.updateCamObjectMat(glm::vec3(mat[3]));
				}


				player->changeState(gameObject.animationstate());

				if (gameObject.hattype() != NO_HAT) {
					player->attachHat((HatType)gameObject.hattype());
				}
				else {
					player->detachHat();
				}
			}

		}
		gameQueue.pop_front();
	}
	gameLock.unlock();
}


void MessageHandler::handleEndGameMessages() {
}