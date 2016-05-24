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
				Globals::gameObjects.hatMap.erase(event.hatid());
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
				model = _Crate; // change
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
				(*map)[id] = Window::createGameObj(model, Globals::gameObjects.modelMap[model]);
			}

			auto& player = *(*map)[id];

			glm::mat4 mat = glm::make_mat4(matrix);

			player.setMatrix(mat);
			if (gameObject.type() == protos::Message_GameObject_Type_PLAYER) {

				if (gameObject.id() == Globals::ID) { // follow camera based player
					glm::mat4 toWorld = Globals::drawData.matrix * mat;
					Globals::cam.updateCamObjectMat(glm::vec3(mat[3]));
				}

				if (gameObject.hattype() != NO_HAT) {
					dynamic_cast<Player*>(&player)->attachHat(WIZARD_HAT);
				}
				else {
					dynamic_cast<Player*>(&player)->detachHat();
				}
			}

		}
		gameQueue.pop_front();
	}
	gameLock.unlock();
}


void MessageHandler::handleEndGameMessages() {
}