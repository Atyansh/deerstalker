#include "Window.h"
#include "client\Globals.h"
#include "LightShader.h"
#include "Mango.h"
#include "Model.h"
#include "SNode.h"
#include "SMatrixTransform.h"
#include "World.h"
#include <glm/ext.hpp>

#include "client\Globals.h"

#include "util\Protos.pb.h"
#include "util\Util.h"

#include <unordered_map>

using namespace util;

const char* window_title = "GLFW Starter Project";

bool cubeMode;

int Window::width;
int Window::height;
std::unordered_map<std::uint32_t, SMatrixTransform*> playerMap;
std::unordered_map<std::uint32_t, std::unique_ptr<Cube>> cubeMap;

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
	Globals::lightShader = new LightShader(Globals::camera.getPosition(), "Graphics/Shaders/shader_lighting.vert", "Graphics/Shaders/shader_lighting.frag");
	Globals::lightShader->addDirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f));
	//Globals::lightShader->addDirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f));
	Globals::lightShader->addPointLight(pointLightPositions[0], glm::vec3(0.05f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.009f, 0.0032f);
	Globals::lightShader->addPointLight(pointLightPositions[1], glm::vec3(0.05f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.009f, 0.0032f);

	World *world = new World();
	Globals::skybox.setupVAO();

	root = new SMatrixTransform();
	root->addNode(world);

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
		Globals::drawData.projection = glm::perspective(45.0f, float(width) / (float)height, 0.1f, 1000.0f);
		//Globals::drawData.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		Globals::drawData.view = Globals::camera.getView();

		cout << "projection in window \n";
		cout << glm::to_string(Globals::drawData.projection) << endl;
	}
}

void Window::idle_callback(GLFWwindow* window) {
	while (!Globals::eventQueue.empty()) {
		protos::TestEvent event = Globals::eventQueue.front();
		Globals::eventQueue.pop_front();

		for (int i = 0; i < event.gameobject_size(); i++){
			auto& gameObject = event.gameobject(i);
			int id = gameObject.id();

			//cout << "Matrix: \n";

			float matrix[16];
			for (int j = 0; j < gameObject.matrix_size(); j++) {
				matrix[j] = gameObject.matrix(j);
				
				/*
				cout << matrix[j] << " ";
				if (j % 4 == 3) {
					cout << endl;
				}*/
			}
			//cout << endl;

			if (cubeMode) {
				if (cubeMap.find(id) == cubeMap.end()) {
					cubeMap[id] = std::make_unique<Cube>(2.0);
				}

				auto& cube = *cubeMap[id];
				cube.toWorld = glm::make_mat4(matrix);
			}
			else {
				if (playerMap.find(id) == playerMap.end()) {
					playerMap[id] = Mango::createNewMango();
				}

				auto& player = *playerMap[id];
				glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(0.5f));
				glm::mat4 mat = glm::make_mat4(matrix);
				player.setMatrix(mat * scale);
			}
		}
	}
}

void Window::display_callback(GLFWwindow* window) {
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
			pair.second->draw(Globals::drawData);
		}

	}

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	protos::TestEvent event;

	event.set_clientid(Globals::ID);

	event.set_type(protos::TestEvent_Type_MOVE);

	bool validEvent = true;

	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_D:
			event.set_direction(protos::RIGHT);
			break;
		case GLFW_KEY_A:
			event.set_direction(protos::LEFT);
			break;
		case GLFW_KEY_W:
			event.set_direction(protos::FORWARD);
			break;
		case GLFW_KEY_S:
			event.set_direction(protos::BACKWARD);
			break;
		case GLFW_KEY_UP:
			event.set_direction(protos::UP);
			break;
		case GLFW_KEY_DOWN:
			event.set_direction(protos::DOWN);
			break;
		default:
			validEvent = false;
		}
		if (validEvent) {
			sendEvent(Globals::socket, std::move(event));
		}
	}
}