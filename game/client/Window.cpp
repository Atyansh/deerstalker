#include "Window.h"
#include "client\Globals.h"
#include "Graphics\LightShader.h"
#include "Game\Model.h"
#include "Game\SNode.h"
#include "Game\SMatrixTransform.h"
#include "Game\World.h"
#include "Game\Player.h"
#include "Game\Hat.h"
#include <glm/ext.hpp>
#include "State.h"
#include "MessageHandler.h"

#include "client\Globals.h"

#include "util\Util.h"
#include "util\Gamepad.h"

#include <unordered_map>

using namespace util;
using namespace Gamepad;

const char* window_title = "Deerstalker";


int Window::width;
int Window::height;
SMatrixTransform *root;

int STATE;

void Window::initialize_objects()
{
	Globals::gameObjects.loadGameObjects();

	STATE = State::_Start;
	cout << "A message for people starting the game and not seeing the character move. Please hit \"START\" the press the A button. Thank you.\n";
	Shader* shader = new Shader("Shaders/fontShader.vert", "Shaders/fontShader.frag");
	Shader* twodShader = new Shader("Shaders/2DShader.vert", "Shaders/2DShader.frag");

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
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, glfwGetPrimaryMonitor(), NULL);
	//GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

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
	Globals::drawData.projection = glm::perspective(45.0f, aspect, 0.1f, 3000.0f);
	Globals::drawData.view = Globals::cam.getView();
	cout << "projection in winodw \n";
	cout << glm::to_string(Globals::drawData.projection) << endl;
}


void Window::idle_callback(GLFWwindow* window) {
	if (STATE == State::_Lobby) {
		MessageHandler::handleLobbyMessages();
	}
	if (STATE == State::_Start) {
		MessageHandler::handleStartMessages();
	}
	if (STATE == State::_Game) {
		MessageHandler::handleGameMessages();
	}
	if (STATE == State::_EndGame) {
		MessageHandler::handleEndGameMessages();
	}
}


void Window::display_callback(GLFWwindow* window) {
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (STATE) {
	case State::_Start:
		Globals::gameObjects.guiMap[_Background]->draw(Globals::drawData);
		
		break;
	case State::_Lobby:
		Globals::gameObjects.guiMap[_LobbyBG]->draw(Globals::drawData);
		break;
	case State::_Game:

		Globals::drawData.view = Globals::cam.getView();

		Globals::gameObjects.root->draw(Globals::drawData);


		// Render objects
		for (auto& pair : Globals::gameObjects.playerMap) {
			pair.second->draw(Globals::drawData);
		}
		// hat
		for (auto& pair : Globals::gameObjects.hatMap) {
			pair.second->draw(Globals::drawData);
		}

		for (auto& pair : Globals::gameObjects.bulletMap) {
			pair.second->draw(Globals::drawData);

		}
		break;
	case State::_EndGame:
		Globals::gameObjects.guiMap[_EndGameBG]->draw(Globals::drawData);
		break;
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
		case GLFW_KEY_ESCAPE:
			Window::clean_up();
			glfwDestroyWindow(window);
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
		Player *player = dynamic_cast<Player*>(Globals::gameObjects.playerMap.find(Globals::ID)->second);

		if (axes[LEFT_STICK_X] > POS_AXIS_TILT &&
			axes[LEFT_STICK_Y] < NEG_AXIS_TILT) {
			addMoveEvent(message, protos::Event_Direction_FR);
		}
		else if (axes[LEFT_STICK_X] > POS_AXIS_TILT &&
			axes[LEFT_STICK_Y] > POS_AXIS_TILT) {
			addMoveEvent(message, protos::Event_Direction_BR);
		}
		else if (axes[LEFT_STICK_X] < NEG_AXIS_TILT &&
			axes[LEFT_STICK_Y] > POS_AXIS_TILT) {
			addMoveEvent(message, protos::Event_Direction_BL);
		}
		else if (axes[LEFT_STICK_X] < NEG_AXIS_TILT &&
			axes[LEFT_STICK_Y] < NEG_AXIS_TILT) {
			addMoveEvent(message, protos::Event_Direction_FL);
		}
		else if (axes[LEFT_STICK_X] > POS_AXIS_TILT) { // Right
			addMoveEvent(message, protos::Event_Direction_RIGHT);
		}
		else if (axes[LEFT_STICK_X] < NEG_AXIS_TILT) { // Left
			addMoveEvent(message, protos::Event_Direction_LEFT);
		}
		else if (axes[LEFT_STICK_Y] > POS_AXIS_TILT) { // Down
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
		}

		if (axes[RIGHT_STICK_Y] > POS_AXIS_TILT) {
			fprintf(stderr, "Going Right\n");
			Globals::cam.yaw(0);
		}

		else if (axes[RIGHT_STICK_Y] < NEG_AXIS_TILT) {
			fprintf(stderr, "Going Left\n");
			Globals::cam.yaw(1);
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
		if (buttons[BUTTON_X] == GLFW_PRESS && !buttonState[BUTTON_X]) {
			auto* event = message.add_event();
			event->set_clientid(Globals::ID);
			event->set_type(protos::Event_Type_PUNCH);
			//buttonState[BUTTON_X] = true;
		}
		if (buttons[BUTTON_Y] == GLFW_PRESS) {
			auto* event = message.add_event();
			event->set_clientid(Globals::ID);
			event->set_type(protos::Event_Type_DQUIP);
		}
		if (buttons[BUTTON_LB] == GLFW_PRESS && !buttonState[BUTTON_LB]) {
			auto* event = message.add_event();
			event->set_clientid(Globals::ID);
			event->set_type(protos::Event_Type_HATL);
			buttonState[BUTTON_LB] = true;
		}
		if (buttons[BUTTON_RB] == GLFW_PRESS && !buttonState[BUTTON_RB]) {
			auto* event = message.add_event();
			event->set_clientid(Globals::ID);
			event->set_type(protos::Event_Type_HATR);
			buttonState[BUTTON_RB] = true;
		}

		if (buttons[BUTTON_A] == GLFW_RELEASE) {
			buttonState[BUTTON_A] = false;
		}
		if (buttons[BUTTON_B] == GLFW_RELEASE) {
			buttonState[BUTTON_B] = false;
		}
		if (buttons[BUTTON_X] == GLFW_RELEASE) {
			buttonState[BUTTON_X] = false;
		}
		if (buttons[BUTTON_Y] == GLFW_RELEASE) {
			buttonState[BUTTON_Y] = false;
		}
		if (buttons[BUTTON_LB] == GLFW_RELEASE) {
			buttonState[BUTTON_LB] = false;
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

SMatrixTransform* Window::createGameObj(Models modelType, Model* model, int id) {
	SMatrixTransform* transform = new SMatrixTransform();
	std::unordered_map<std::uint32_t, Hat*> playerHatMap;
	std::unordered_map<std::uint32_t, PlayerAnim*> playerStateMap;
	switch (modelType) {
		case _Player:
			playerHatMap[WIZARD_HAT] = new Hat(Globals::gameObjects.modelMap[_WizardHat]);
			playerHatMap[PROPELLER_HAT] = new Hat(Globals::gameObjects.modelMap[_PropellerHat]);
			playerHatMap[BEAR_HAT] = new Hat(Globals::gameObjects.modelMap[_BearHat]);
			playerHatMap[HARD_HAT] = new Hat(Globals::gameObjects.modelMap[_HardHat]);
			playerHatMap[DEERSTALKER_HAT] = new Hat(Globals::gameObjects.modelMap[_DeerstalkerHat]);
			playerHatMap[CRATE] = new Hat(Globals::gameObjects.modelMap[_Crate]);
			playerStateMap[protos::Message_GameObject_AnimationState_STANDING] = new PlayerAnim(dynamic_cast<PlayerModel*>(Globals::gameObjects.modelMap[_Player_Standing]), id);
			playerStateMap[protos::Message_GameObject_AnimationState_RUNNING] = new PlayerAnim(dynamic_cast<PlayerModel*>(Globals::gameObjects.modelMap[_Player_Running]), id);
			playerStateMap[protos::Message_GameObject_AnimationState_PUNCHING] = new PlayerAnim(dynamic_cast<PlayerModel*>(Globals::gameObjects.modelMap[_Player_Punching]), id);
			return new Player(playerStateMap, playerHatMap);
		default:
			transform->addNode(model);
			break;
	}
	return transform;
}