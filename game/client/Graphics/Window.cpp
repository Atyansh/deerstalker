#include "Window.h"
#include "Globals.h"
#include "LightShader.h"
#include "Mango.h"
#include "Model.h"
#include "SNode.h"
#include "SMatrixTransform.h"
#include "util\Message.h"
#include <glm/ext.hpp>
#include "Hashtable.cpp"	// Need this to fix linker problem. Ask AJ later
#include "util\ModelKeys.h"



using namespace util;

enum { max_length = 1024 };

const char* window_title = "GLFW Starter Project";

int Window::width;
int Window::height;
bool Window::holdUp;
bool Window::holdDown;
bool Window::holdLeft;
bool Window::holdRight;
bool Window::holdLRot;
bool Window::holdRRot;

Mango *guy;
//Model *ourModel;

void Window::initialize_objects()
{
	guy = Mango::createNewMango();

	glm::mat4 loc = glm::translate(glm::mat4(), glm::vec3(0.0f, -0.5f, -20.0f));
	loc = glm::scale(loc, glm::vec3(0.8f));
	Globals::drawData.matrix = loc;
}

void Window::clean_up()
{
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
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

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size
	glViewport(0, 0, width, height);
	Globals::drawData.projection = glm::perspective(45.0f, float(width) / (float)height, 0.1f, 100.0f);
	//Globals::drawData.view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Globals::drawData.view = Globals::camera.getView();

	cout << "projection in window \n";
	cout << glm::to_string(Globals::drawData.projection) << endl;
}

void Window::idle_callback(GLFWwindow* window)
{
	// Perform any updates as necessary. Here, we will spin the cube slightly.
	guy->update(Globals::updateData);

	while (!Globals::keyQueue.empty()) {
		char keyPress = Globals::keyQueue.front();
		Globals::keyQueue.pop_front();
		int action = Globals::keyQueue.front()-1;
		Globals::keyQueue.pop_front();
		if (action == GLFW_PRESS)
		{
			switch (keyPress){
			case GLFW_KEY_ESCAPE:
				// Close the window. This causes the program to also terminate.
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
			case GLFW_KEY_D:
				//move right
				holdRight = true;
				break;
			case GLFW_KEY_A:
				//move left
				holdLeft = true;
				break;
			case GLFW_KEY_W:
				//move right
				holdUp = true;
				break;
			case GLFW_KEY_S:
				//move left
				holdDown = true;
				break;
			case GLFW_KEY_Q:
				//rotate left
				holdLRot = true;
				break;
			case GLFW_KEY_E:
				//rotate right
				holdRRot = true;
				break;
			}
		}
		if (action == GLFW_RELEASE)
		{
			switch (keyPress){
			case GLFW_KEY_ESCAPE:
				// Close the window. This causes the program to also terminate.
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
			case GLFW_KEY_D:
				//move right
				holdRight = false;
				break;
			case GLFW_KEY_A:
				//move left
				holdLeft = false;
				break;
			case GLFW_KEY_W:
				//move right
				holdUp = false;
				break;
			case GLFW_KEY_S:
				//move left
				holdDown = false;
				break;
			case GLFW_KEY_Q:
				//rotate left
				holdLRot = false;
				break;
			case GLFW_KEY_E:
				//rotate right
				holdRRot = false;
				break;
			}
		}
	}
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Movement
	calcMovements();

	// Render objects
	guy->draw(Globals::drawData);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::calcMovements() {
	glm::mat4 change(1.0f);
	if (holdLRot == true) {
		//glm::mat4 rot = glm::rotate(glm::mat4(), -1.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
		guy->setMatrix(guy->getDrawData().matrix * glm::rotate(glm::mat4(1.0f), -1.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)));
		
	}
	if (holdRRot == true) {
		//glm::mat4 rot = glm::rotate(glm::mat4(), 1.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
		guy->setMatrix(guy->getDrawData().matrix * glm::rotate(glm::mat4(1.0f), 1.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)));
		
	}
	if (holdDown == true) {
		change = glm::translate(change, glm::vec3(0.0f, -0.2f, 0.0f));
	}
	if (holdUp == true) {
		change = glm::translate(change, glm::vec3(0.0f, 0.2f, 0.0f));
	}
	if (holdRight == true) {
		change = glm::translate(change, glm::vec3(0.2f, 0.0f, 0.0f));
	}
	if (holdLeft == true) {
		change = glm::translate(change, glm::vec3(-0.2f, 0.0f, 0.0f));
	}
	//ourModel->toWorld = change * ourModel->toWorld;
	
	Globals::drawData.matrix = change * Globals::drawData.matrix;
	
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Server
	char request[max_length];

	request[0] = glfwToAscii(key);
	request[1] = (char) action+1;
	request[2] = '\0';

	Message m(request);
	boost::asio::write(Globals::socket, boost::asio::buffer(m.data(), m.length()));
}

int Window::glfwToAscii(const int key){
	switch (key){
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

int Window::asciiToGLFW(const int key){
	switch (key){
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