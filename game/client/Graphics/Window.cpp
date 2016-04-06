#include "Window.h"
#include "Globals.h"

enum { max_length = 1024 };

const char* window_title = "GLFW Starter Project";
Cube cube(5.0f);

int Window::width;
int Window::height;
bool Window::holdUp;
bool Window::holdDown;
bool Window::holdLeft;
bool Window::holdRight;

void Window::initialize_objects()
{
	holdUp = false;
	holdDown = false;
	holdLeft = false;
	holdRight = false;
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
	// Set the matrix mode to GL_PROJECTION to determine the proper camera properties
	glMatrixMode(GL_PROJECTION);
	// Load the identity matrix
	glLoadIdentity();
	// Set the perspective of the projection viewing frustum
	gluPerspective(60.0, double(width) / (double)height, 1.0, 1000.0);
	// Move camera back 20 units so that it looks at the origin (or else it's in the origin)
	glTranslatef(0, 0, -20);
}

void Window::idle_callback()
{
	// Perform any updates as necessary. Here, we will spin the cube slightly.
	cube.update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set the matrix mode to GL_MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	// Load the identity matrix
	glLoadIdentity();

	//Movement
	calcMovements();
	
	// Render objects
	cube.draw();

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::calcMovements() {
	glm::mat4 change(1.0f);
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
	cube.toWorld = change * cube.toWorld;

}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Server
	char request[max_length];
	char reply[max_length];

	size_t request_length;
	size_t reply_length;

	request[0] = glfwToAscii(key);
	request[1] = '\0';
	request_length = std::strlen(request);

	std::cout << "Request is: ";
	std::cout.write(request, request_length);
	std::cout << " with key: " << glfwToAscii(key);
	std::cout << "\n";


	boost::asio::write(Globals::socket, boost::asio::buffer(request, request_length));

	//server
	reply_length = boost::asio::read(Globals::socket,
		boost::asio::buffer(reply, request_length));

	int keyPress = asciiToGLFW(reply[0]);

	std::cout << "Reply is: ";
	std::cout.write(reply, reply_length);
	std::cout << " with key: " << keyPress;
	std::cout << "\n";

	if (action == GLFW_PRESS)
	{
		switch (keyPress){
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			std::cout << "die ";
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
		}
	}
	if (action == GLFW_RELEASE)
	{
		switch (keyPress){
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			std::cout << "die 2";
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
		}
	}
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