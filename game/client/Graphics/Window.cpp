#include "Window.h"
#include "Globals.h"
#include "util\Message.h"
#include <unordered_map>

using namespace util;

enum { max_length = 1024 };

const char* window_title = "GLFW Starter Project";

int Window::width;
int Window::height;
std::unordered_map<char, std::unique_ptr<Cube>> cubeMap;

void Window::initialize_objects() {
	cubeMap['1'] = std::make_unique<Cube>(5.0f);
	cubeMap['2'] = std::make_unique<Cube>(5.0f);

	glm::mat4 change1(1.0f);
	change1 = glm::translate(change1, glm::vec3(-8.0f, 0.0f, 0.0f));
	cubeMap['1']->toWorld *= change1;

	glm::mat4 change2(1.0f);
	change2 = glm::translate(change2, glm::vec3(8.0f, 0.0f, 0.0f));
	cubeMap['2']->toWorld *= change2;
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
	for (auto& pair : cubeMap) {
		pair.second->update();
	}
	
	while (!Globals::keyQueue.empty()) {
		Cube& cube = *cubeMap[Globals::keyQueue.front()];
		Globals::keyQueue.pop_front();
		char keyPress = Globals::keyQueue.front();
		Globals::keyQueue.pop_front();
		int action = Globals::keyQueue.front()-1;
		Globals::keyQueue.pop_front();
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

	//Movement
	for (auto& pair : cubeMap) {
		calcMovements(*pair.second);
	}
	
	// Render objects
	for (auto& pair : cubeMap) {
		pair.second->draw();
	}

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::calcMovements(Cube& cube) {
	glm::mat4 change(1.0f);
	if (cube.holdDown == true) {
		change = glm::translate(change, glm::vec3(0.0f, -0.2f, 0.0f));
	}
	if (cube.holdUp == true) {
		change = glm::translate(change, glm::vec3(0.0f, 0.2f, 0.0f));
	}
	if (cube.holdRight == true) {
		change = glm::translate(change, glm::vec3(0.2f, 0.0f, 0.0f));
	}
	if (cube.holdLeft == true) {
		change = glm::translate(change, glm::vec3(-0.2f, 0.0f, 0.0f));
	}
	cube.toWorld = change * cube.toWorld;
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//Server
	char request[max_length];

	request[0] = Globals::ID;
	request[1] = (char) glfwToAscii(key);
	request[2] = (char) action+1;
	request[3] = '\0';

	Message m(request);
	std::cerr << "AJ length: " << m.length() << std::endl;
	std::cerr << "AJ body_length: " << m.body_length() << std::endl;
	boost::asio::write(Globals::socket, boost::asio::buffer(m.data(), m.length()));
}

int Window::glfwToAscii(const int key) {
	switch (key) {
	case GLFW_KEY_ESCAPE:
		return ESC;
	case GLFW_KEY_ENTER:
		return ENTER;
	case GLFW_KEY_SPACE:
		return SPACE;
	default:
		return key;
	}
}

int Window::asciiToGLFW(const int key) {
	switch (key) {
	case ESC:
		return GLFW_KEY_ESCAPE;
	case ENTER:
		return GLFW_KEY_ENTER;
	case SPACE:
		return GLFW_KEY_SPACE;
	default:
		return key;
	}
}