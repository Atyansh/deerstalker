#include "Window.h"
#include "client\Globals.h"
#include "Graphics\LightShader.h"
#include "Graphics\Model.h"
#include "Graphics\SNode.h"
#include "Graphics\SMatrixTransform.h"
#include "Game\World.h"
#include "Game\Player.h"
#include "Game\Hat.h"
#include <glm/ext.hpp>
#include "State.h"

#include "client\Globals.h"

#include "util\Util.h"
#include "util\Gamepad.h"

#include <unordered_map>

using namespace util;
using namespace Gamepad;

const char* window_title = "Deerstalker";
//string skyboxDirectory = "Assets/Cubemap";


int Window::width;
int Window::height;
//std::unordered_map<std::uint32_t, SMatrixTransform*> playerMap;
//std::unordered_map<std::uint32_t, SMatrixTransform*> hatMap;
//std::unordered_map<std::uint32_t, Model*> modelMap;
//std::unordered_map<std::uint32_t, Shader*> shaderMap;
//
//const char* mangoPath = "Assets/OBJ/Mango/mango.obj";
//const char* chickenPath = "Assets/FBX/chicken_dance.fbx";
//const char* cratePath = "Assets/OBJ/Crate/Crate1.obj";
//const char* playerPath = "Assets/OBJ/Player/Player.obj";
//const char* wizardPath = "Assets/OBJ/Wizard_Hat/wizard_hat.obj";


//GameObjects _gameObjects;

SMatrixTransform *root;

int STATE;

void Window::initialize_objects()
{
	/*glm::vec3 pointLightPositions[] = {
		glm::vec3(2.3f, -1.6f, -3.0f),
		glm::vec3(-1.7f, 0.9f, 1.0f)
	};

	LightShader* lightShader = new LightShader(Globals::cam.getPosition(), "Shaders/shader_lighting.vert", "Shaders/shader_lighting.frag");
	lightShader->addDirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f));
	//lightShader->addDirectionalLight(glm::vec3(0.2f, 10.0f, 0.f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f));
	lightShader->addPointLight(pointLightPositions[0], glm::vec3(0.05f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.009f, 0.0032f);
	lightShader->addPointLight(pointLightPositions[1], glm::vec3(0.05f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.009f, 0.0032f);

	shaderMap[_LtShader] = lightShader;

	modelMap[_Mango] = new Model(mangoPath, shaderMap[_LtShader]);
	modelMap[_Player] = new PlayerModel(playerPath, shaderMap[_LtShader]);
	modelMap[_Crate] = new Model(cratePath, shaderMap[_LtShader]);
	modelMap[_Wizard] = new Model(wizardPath, shaderMap[_LtShader]);
	*/

	Globals::gameObjects.loadGameObjects();

	//Window::generateWorld(skyboxDirectory);


	STATE = State::_Start;
	cout << "A message for people starting the game and not seeing the character move. Please hit \"START\" the press the A button. Thank you.\n";

}

void Window::clean_up() {
	// TODO(Atyansh): Deal with cleaning up stuff.
}

GLFWwindow* Window::create_window(int width, int height) {
	// Initialize GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height) {
	Window::width = width;
	Window::height = height;
	// Set the viewport size
	glViewport(0, 0, width, height);
	float aspect = height == 0 ? 0 : float(width) / (float)height;
	Globals::drawData.projection = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
	//Globals::drawData.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Globals::drawData.view = Globals::cam.getView();

	cout << "projection in winodw \n";
	cout << glm::to_string(Globals::drawData.projection) << endl;
}

// This method handles client side event logic and Controller input parsing
void Window::idle_callback(GLFWwindow* window) {
	Globals::queueLock.lock();
	while (!Globals::messageQueue.empty()) {
		protos::Message& message = Globals::messageQueue.front();
		
		for (int i = 0; i < message.event_size(); i++) {
			auto& event = message.event(i);
			
			if (event.type() == protos::Event_Type_EQUIP) {
				Globals::gameObjects.hatMap.erase(event.hatid());
			}
		}

		for (int i = 0; i < message.gameobject_size(); i++){
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

			/*glm::mat4 mat2 = glm::scale(mat, glm::vec3(.01f));
			player.setMatrix(mat2);*/

			player.setMatrix(mat);
			if (gameObject.type() == protos::Message_GameObject_Type_PLAYER) {

				if (gameObject.id() == Globals::ID) { // follow camera based player
					glm::mat4 toWorld = Globals::drawData.matrix * mat;
					Globals::cam.updateCamObjectMat(glm::vec3(mat[3]));
				}

				if (gameObject.hattype() != 0) {
					switch (id) // attach hats
					{
					case 1:
						dynamic_cast<Player*>(&player)->attachHat(_wizard);
						break;
					default:
						dynamic_cast<Player*>(&player)->attachHat(_crate);
						break;
					}
				}
				else {
					dynamic_cast<Player*>(&player)->detachHat();
				}
			}

		}

		Globals::messageQueue.pop_front();
	}

	Globals::queueLock.unlock();
}

void Window::display_callback(GLFWwindow* window) {
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Globals::drawData.view = Globals::cam.getView();

	Globals::gameObjects.root->draw(Globals::drawData);


	// Render objects
	for (auto& pair : Globals::gameObjects.playerMap) {
		//pair.second->draw(Globals::drawData);
		//glm::mat4 toWorld = Globals::drawData.matrix * pair.second->getDrawData().matrix;
		pair.second->draw(Globals::drawData);
	}
	// hat
	for (auto& pair : Globals::gameObjects.hatMap) {
		pair.second->draw(Globals::drawData);
	}
	
	for (auto& pair : Globals::gameObjects.bulletMap) {
		//pair.second->draw(Globals::drawData);
		//glm::mat4 toWorld = Globals::drawData.matrix * pair.second->getDrawData().matrix;
		pair.second->draw(Globals::drawData);

	}

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	protos::Message message;

	auto event = message.add_event();

	event->set_clientid(Globals::ID);

	event->set_type(protos::Event_Type_MOVE);

	glm::vec3 camDir = Globals::cam.getCamDirection();
	event->add_cameravector(camDir.x);
	event->add_cameravector(camDir.y);
	event->add_cameravector(camDir.z);

	bool validEvent = true;

	if (action == GLFW_PRESS) {
		switch (key) {
			cout << key << endl;
		case GLFW_KEY_D:
			event->set_direction(protos::Event_Direction_RIGHT);
			break;
		case GLFW_KEY_A:
			event->set_direction(protos::Event_Direction_LEFT);
			break;
		case GLFW_KEY_W:
			event->set_direction(protos::Event_Direction_FORWARD);
			break;
		case GLFW_KEY_S:
			event->set_direction(protos::Event_Direction_BACKWARD);
			break;
		case GLFW_KEY_UP:
			event->set_direction(protos::Event_Direction_UP);
			break;
		case GLFW_KEY_DOWN:
			event->set_direction(protos::Event_Direction_DOWN);
			break;
		default:
			validEvent = false;
		}


		if (validEvent) {
			sendMessage(Globals::socket, std::move(message));
		}
	}
}

void Window::handle_gamepad(GLFWwindow* window) {

	protos::Message message;

	int count = 0;
	auto* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
	auto* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

	if (STATE == State::_Game) {
		if (axes[LEFT_STICK_X] > POS_AXIS_TILT) { // Right
			addMoveEvent(message, protos::Event_Direction_RIGHT);
		}
		else if (axes[LEFT_STICK_X] < NEG_AXIS_TILT) { // Left
			addMoveEvent(message, protos::Event_Direction_LEFT);
		}

		if (axes[LEFT_STICK_Y] > POS_AXIS_TILT) { // Down
			addMoveEvent(message, protos::Event_Direction_BACKWARD);
		}
		else if (axes[LEFT_STICK_Y] < NEG_AXIS_TILT) { // Up
			addMoveEvent(message, protos::Event_Direction_FORWARD);
		}

		if (axes[TRIGGER_AXIS] > POS_AXIS_TILT) {
			addMoveEvent(message, protos::Event_Direction_DOWN);
		}
		else if (axes[TRIGGER_AXIS] < NEG_AXIS_TILT) {
			addMoveEvent(message, protos::Event_Direction_UP);
		}

		if (axes[RIGHT_STICK_X] > POS_AXIS_TILT) {
			fprintf(stderr, "Going Down\n");
			Globals::cam.pitch(0);
		}
		else if (axes[RIGHT_STICK_X] < NEG_AXIS_TILT) {
			fprintf(stderr, "Going Up\n");
			Globals::cam.pitch(1);
			//Globals::drawData.view = Globals::cam.getView();
		}

		if (axes[RIGHT_STICK_Y] > POS_AXIS_TILT) {
			fprintf(stderr, "Going Right\n");
			Globals::cam.yaw(0);
			//Globals::drawData.view = Globals::cam.getView();
		}

		else if (axes[RIGHT_STICK_Y] < NEG_AXIS_TILT) {
			fprintf(stderr, "Going Left\n");
			Globals::cam.yaw(1);
			//Globals::drawData.view = Globals::cam.getView();
		}



		if (buttons[BUTTON_A] == GLFW_PRESS && !buttonState[BUTTON_A]) {
			auto* event = message.add_event();
			event->set_clientid(Globals::ID);
			event->set_type(protos::Event_Type_JUMP);
			buttonState[BUTTON_A] = true;
		}
		if (buttons[BUTTON_B] == GLFW_PRESS && !buttonState[BUTTON_B]) {
			auto* event = message.add_event();
			event->set_clientid(Globals::ID);
			event->set_type(protos::Event_Type_EQUIP);
			buttonState[BUTTON_B] = true;
		}
		if (buttons[BUTTON_Y] == GLFW_PRESS) {
			auto* event = message.add_event();
			event->set_clientid(Globals::ID);
			event->set_type(protos::Event_Type_DQUIP);
		}
		if (buttons[BUTTON_RB] == GLFW_PRESS && !buttonState[BUTTON_RB]) {
			auto* event = message.add_event();
			event->set_clientid(Globals::ID);
			event->set_type(protos::Event_Type_SHOOT);
			buttonState[BUTTON_RB] = true;
		}

		if (buttons[BUTTON_A] == GLFW_RELEASE) {
			buttonState[BUTTON_A] = false;
		}
		if (buttons[BUTTON_B] == GLFW_RELEASE) {
			buttonState[BUTTON_B] = false;
		}
		if (buttons[BUTTON_Y] == GLFW_RELEASE) {
			buttonState[BUTTON_Y] = false;
		}
		if (buttons[BUTTON_RB] == GLFW_RELEASE) {
			buttonState[BUTTON_RB] = false;
		}
	}

	if (STATE == State::_Start) {
		if (buttons[BUTTON_START] == GLFW_PRESS) {

			// Switch state to lobby
			STATE = State::_Lobby;
		}
	}

	if (STATE == State::_Lobby) {
		if (buttons[BUTTON_A] == GLFW_PRESS) {
			STATE = State::_Game;
		}
	}

	if (STATE == State::_EndGame) {
		if (buttons[BUTTON_Y] == GLFW_PRESS) {
			STATE = State::_Game;
		}
		else if (buttons[BUTTON_X] == GLFW_PRESS) {
			STATE = State::_Start;
		}
	}


	if (message.event_size()) {
		sendMessage(Globals::socket, message);
	}

}

void Window::addMoveEvent(protos::Message& message, protos::Event_Direction direction) {
	protos::Event* event = message.add_event();
	event->set_clientid(Globals::ID);
	event->set_type(protos::Event_Type_MOVE);
	event->set_direction(direction);

	glm::vec3 camDir = Globals::cam.getCamDirection();
	event->add_cameravector(camDir.x);
	event->add_cameravector(camDir.y);
	event->add_cameravector(camDir.z);
}

SMatrixTransform* Window::createGameObj(Models modelType, Model* model) {
	SMatrixTransform* transform = new SMatrixTransform();
	std::unordered_map<std::uint32_t, Hat*> playerHatMap;
	switch (modelType)
	{
		case _Player:
			playerHatMap[_wizard] = new Hat(_wizard, Globals::gameObjects.modelMap[_Wizard]);
			playerHatMap[_crate] = new Hat(_crate, Globals::gameObjects.modelMap[_Crate]);
			return new Player(dynamic_cast<PlayerModel*>(model), playerHatMap);
		default:
			transform->addNode(model);
			break;
	}
	return transform;
}

//void Window::generateWorld(string directory) {
//	World *world = new World();
//	world->createWorld(Globals::gameObjects.shaderMap[_LtShader], directory);
//	cerr << "A" << endl;;
//	root = new SMatrixTransform();
//	root->addNode(world);
//}
