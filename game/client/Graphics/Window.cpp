#include "Window.h"
#include <unordered_map>

#include "client\Globals.h"

#include "util\Protos.pb.h"
#include "util\Util.h"

using namespace util;

const char* window_title = "GLFW Starter Project";

int Window::width;
int Window::height;
std::unordered_map<std::uint32_t, std::unique_ptr<Cube>> cubeMap;

void Window::initialize_objects() {
}

void Window::clean_up() {
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
	// Set the matrix mode to GL_PROJECTION to determine the proper camera properties
	glMatrixMode(GL_PROJECTION);
	// Load the identity matrix
	glLoadIdentity();
	// Set the perspective of the projection viewing frustum
	gluPerspective(60.0, double(width) / (double)height, 1.0, 1000.0);
	// Move camera back 20 units so that it looks at the origin (or else it's in the origin)
	glTranslatef(0, 0, -20);
}

void Window::idle_callback(GLFWwindow* window) {
	// Perform any updates as necessary. Here, we will spin the cube slightly.
	//for (auto& pair : cubeMap) {
	//	pair.second->update();
	//}
	
	while (!Globals::eventQueue.empty()) {
		protos::TestEvent event = Globals::eventQueue.front();
		Globals::eventQueue.pop_front();

		for (int i = 0; i < event.gameobject_size(); i++){
			auto& gameObject = event.gameobject(i);
			int id = gameObject.id();
			if (cubeMap.find(id) == cubeMap.end()) {
				cubeMap[id] = std::make_unique<Cube>(2.0f);
			}

			Cube& cube = *cubeMap[id];
			float matrix[16];
			for (int j = 0; j < gameObject.matrix_size(); j++) {
				matrix[j] = gameObject.matrix(j);
			}
			cube.toWorld = glm::make_mat4(matrix);

			// TODO(Atyansh): Didn't need to transpose. Someone was wrong.
			//cube.toWorld = glm::transpose(cube.toWorld);
		}
	}
}

void Window::display_callback(GLFWwindow* window) {
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set the matrix mode to GL_MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	// Load the identity matrix
	glLoadIdentity();
	
	// Render objects
	for (auto& pair : cubeMap) {
		pair.second->draw();
	}

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	protos::TestEvent event;

	//event.set_action(action);
	event.set_clientid(Globals::ID);
	//event.set_keypress(key);

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

	/*
	if (action == GLFW_PRESS) {
		switch (keyPress) {
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_D:
			//move right
			cube.holdRight = true;
			break;
		case GLFW_KEY_A:
			//move left
			cube.holdLeft = true;
			break;
		case GLFW_KEY_W:
			//move right
			cube.holdUp = true;
			break;
		case GLFW_KEY_S:
			//move left
			cube.holdDown = true;
			break;
		}
	}
	if (action == GLFW_RELEASE) {
		switch (keyPress) {
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_D:
			//move right
			cube.holdRight = false;
			break;
		case GLFW_KEY_A:
			//move left
			cube.holdLeft = false;
			break;
		case GLFW_KEY_W:
			//move right
			cube.holdUp = false;
			break;
		case GLFW_KEY_S:
			//move left
			cube.holdDown = false;
			break;
		}
	}*/
}