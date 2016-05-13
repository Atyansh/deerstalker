#include "Window.h"
#include "client\Globals.h"
#include "LightShader.h"
#include "Model.h"
#include "SNode.h"
#include "SMatrixTransform.h"
#include "World.h"
#include <glm/ext.hpp>

#include "client\Globals.h"

#include "util\Util.h"
#include "util\Gamepad.h"

#include <unordered_map>

using namespace util;
using namespace Gamepad;

const char* window_title = "Deerstalker";
string skyboxDirectory = "Graphics/Assets/Cubemap";

bool cubeMode;

enum Models {
	_Player, 
	_Mango,
	_Crate,
	_PokeBall
};

enum Shaders {
	_BShader,
	_LtShader
};

int Window::width;
int Window::height;
std::unordered_map<std::uint32_t, SMatrixTransform*> playerMap;
std::unordered_map<std::uint32_t, SMatrixTransform*> hatMap;
std::unordered_map<std::uint32_t, std::unique_ptr<Cube>> cubeMap;
std::unordered_map<std::uint32_t, Model*> modelMap;
std::unordered_map<std::uint32_t, Shader*> shaderMap;

const char* mangoPath = "Graphics/Assets/OBJ/Mango/mango.obj";
const char* chickenPath = "Graphics/Assets/FBX/chicken_dance.fbx";
const char* cratePath = "Graphics/Assets/OBJ/Crate/Crate1.obj";

SMatrixTransform *root;

void Window::initialize_objects()
{
	glm::mat4 loc = glm::translate(glm::mat4(), glm::vec3(0.0f, -0.5f, -20.0f));
	loc = glm::scale(loc, glm::vec3(0.8f));
	Globals::drawData.matrix = loc;

	glm::vec3 pointLightPositions[] = {
		glm::vec3(2.3f, -1.6f, -3.0f),
		glm::vec3(-1.7f, 0.9f, 1.0f)
	};

	LightShader* lightShader = new LightShader(Globals::cam.getPosition(), "Graphics/Shaders/shader_lighting.vert", "Graphics/Shaders/shader_lighting.frag");
	lightShader->addDirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f));
	lightShader->addDirectionalLight(glm::vec3(0.2f, 10.0f, 0.f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f));
	lightShader->addPointLight(pointLightPositions[0], glm::vec3(0.05f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.009f, 0.0032f);
	lightShader->addPointLight(pointLightPositions[1], glm::vec3(0.05f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.009f, 0.0032f);

	shaderMap[_LtShader] = lightShader;

	modelMap[_Mango] = new Model(mangoPath, shaderMap[_LtShader]);
	modelMap[_Crate] = new Model(cratePath, shaderMap[_LtShader]);

	Window::generateWorld(skyboxDirectory);

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

	if (cubeMode) {
		// Set the matrix mode to GL_PROJECTION to determine the proper camera properties
		glMatrixMode(GL_PROJECTION);
		// Load the identity matrix
		glLoadIdentity();
		// Set the perspective of the projection viewing frustum
		gluPerspective(60.0, double(width) / (double)height, 1.0, 1000.0);
		// Move camera back 20 units so that it looks at the origin (or else it's in the origin)
		glTranslatef(0, 0, -20);
	}
	else {
		float aspect = height == 0 ? 0 : float(width) / (float)height;
		Globals::drawData.projection = glm::perspective(45.0f, aspect, 0.1f, 1000.0f);
		//Globals::drawData.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		Globals::drawData.view = Globals::cam.getView();

		cout << "projection in winodw \n";
		cout << glm::to_string(Globals::drawData.projection) << endl;
	}
}

// This method handles client side event logic and Controller input parsing
void Window::idle_callback(GLFWwindow* window) {
	while (!Globals::messageQueue.empty()) {
		protos::Message message = Globals::messageQueue.front();
		Globals::messageQueue.pop_front();

		for (int i = 0; i < message.gameobject_size(); i++){
			Models model;
			auto& gameObject = message.gameobject(i);
			int id = gameObject.id();

			auto* map = &playerMap;

			if (gameObject.type() == protos::Message_GameObject_Type_PLAYER) {
				map = &playerMap;
				model = _Mango;
			}
			else if (gameObject.type() == protos::Message_GameObject_Type_HAT) {
				map = &hatMap;
				model = _Crate;
			}

			float matrix[16];
			for (int j = 0; j < gameObject.matrix_size(); j++) {
				matrix[j] = gameObject.matrix(j);
			}

			if (cubeMode) {
				if (cubeMap.find(id) == cubeMap.end()) {
					cubeMap[id] = std::make_unique<Cube>(2.0);
				}

				auto& cube = *cubeMap[id];
				cube.toWorld = glm::make_mat4(matrix);
			}
			else {
				if ((*map).find(id) == (*map).end()) {
					(*map)[id] = Window::createGameObj(modelMap[model]);
				}

				auto& player = *(*map)[id];
				glm::mat4 mat = glm::make_mat4(matrix);

				/*glm::mat4 mat2 = glm::scale(mat, glm::vec3(.01f));
				player.setMatrix(mat2);*/

				player.setMatrix(mat);
				if (gameObject.type() == protos::Message_GameObject_Type_PLAYER &&
					gameObject.id() == Globals::ID) {
					glm::mat4 toWorld = Globals::drawData.matrix * mat;
					Globals::cam.updateCamObjectMat(glm::vec3(toWorld[3]));
				}
			}
		}
	}
}

void Window::display_callback(GLFWwindow* window) {
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Globals::drawData.view = Globals::cam.getView();
	Globals::skybox.draw();

		

	root->draw(Globals::drawData);

	if (cubeMode) {
		// Set the matrix mode to GL_MODELVIEW
		glMatrixMode(GL_MODELVIEW);
		// Load the identity matrix
		glLoadIdentity();

		// Render objects
		for (auto& pair : cubeMap) {
			pair.second->draw();
		}
	}
	else {
		// Render objects
		for (auto& pair : playerMap) {
			//pair.second->draw(Globals::drawData);
			glm::mat4 toWorld = Globals::drawData.matrix * pair.second->getDrawData().matrix;
			Globals::cam.updateCamObjectMat(glm::vec3(toWorld[3]));
			pair.second->draw(Globals::drawData);

		}
		// hat
		for (auto& pair : hatMap) {
			pair.second->draw(Globals::drawData);
		}
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

	bool validEvent = true;

	int count = 0;
	auto* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

	for (int i = 0; i < count; i++) {
	}

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

	auto* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

	if (buttons[BUTTON_A] == GLFW_PRESS) {
		auto* event = message.add_event();
		event->set_clientid(Globals::ID);
		event->set_type(protos::Event_Type_JUMP);
	}

	if (message.event_size()) {
		sendMessage(Globals::socket, std::move(message));
	}

}

void Window::addMoveEvent(protos::Message& message, protos::Event_Direction direction) {
	protos::Event* event = message.add_event();
	event->set_clientid(Globals::ID);
	event->set_type(protos::Event_Type_MOVE);
	event->set_direction(direction);
}

SMatrixTransform* Window::createGameObj(Model* model) {
	SMatrixTransform* transform = new SMatrixTransform();
	transform->addNode(model);
	return transform;
}

void Window::generateWorld(string directory) {
	World *world = new World();
	world->createWorld(shaderMap[_LtShader]);
	Globals::skybox.setupVAO(directory);
	cerr << "A" << endl;;
	root = new SMatrixTransform();
	root->addNode(world);
}

glm::mat4 Window::moveBasedOnCamera(int direction) {
	// 0 = up
	// 1 = down
	// 2 = left
	// 3 = right

	glm::vec3 b(0.0f, 0.0f, -1.0f);

	//switch (direction) {
	//case 0: // up
	//	b = glm::vec3(0.0f, 0.0f, -1.0f);
	//	break;
	//case 1: // down
	//	b = glm::vec3(0.0f, 0.0f, 1.0f);
	//	break;
	//case 2:
	//	b = glm::vec3(-1.0f, 0.0f, 0.0f);
	//	break;
	//case 3:
	//	b = glm::vec3(1.0f, 0.0f, 0.0f);
	//	break;
	//}

	glm::vec3 a = Globals::cam.getCamDirection();
	cout << glm::to_string(a) << endl;

	float angle = acosf(glm::dot(a, b));

	if (angle > 180) {
		angle -= 180;
	}
	cout << "Angle: " << angle << endl;


	glm::mat4 rot = glm::rotate(glm::mat4(), angle, glm::vec3(0.0f, 1.0f, 0.0f));
	return rot;




}