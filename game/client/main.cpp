#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include "Window.h"
#include "Globals.h"

#include <boost/asio.hpp>

#include "util\ConfigSettings.h"
#include "util\Protos.pb.h"
#include "util\Util.h"

#include "MessageHandler.h"

using boost::asio::ip::tcp;
using namespace util;

GLFWwindow* window;

void error_callback(int error, const char* description) {
	// Print error
	fputs(description, stderr);
}

void setup_callbacks() {
	// Set the error callback
	glfwSetErrorCallback(error_callback);
	// Set the key callback
	glfwSetKeyCallback(window, Window::key_callback);
	// Set the window resize callback
	glfwSetWindowSizeCallback(window, Window::resize_callback);
}

void setup_materials() {
	float specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float shininess[] = { 100.0 };

	// Enable color materials
	glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void setup_lighting() {
	// Enable lighting
	glEnable(GL_LIGHTING);
	// Enable Local Viewer Light Model
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	// Lightsource position
	float position[] = { 10.0, 10.0, 10.0, 1.0 };

	// Generate light source
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	// Enable GL_LIGHT0
	glEnable(GL_LIGHT0);
}

void setup_opengl_settings() {
	// Enable depth buffering
	glEnable(GL_DEPTH_TEST);
	// Related to shaders and z value comparisons for the depth buffer
	glDepthFunc(GL_LEQUAL);
	// Set polygon drawing mode to fill front and back of each polygon
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Disable backface culling to render both sides of polygons
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Set clear color to black
	glClearColor(0.0, 0.0, 0.0, 0.0);
	// Set shading to smooth
	glShadeModel(GL_SMOOTH);
	// Auto normalize surface normals
	glEnable(GL_NORMALIZE);

	// Setup materials
	// setup_materials();
	// Setup lighting
	// setup_lighting();
}

void print_versions() {
	// Get info of GPU and supported OpenGL version
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

	//If the shading language symbol is defined
#ifdef GL_SHADING_LANGUAGE_VERSION
	std::printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif
}

enum { max_length = 1024 };

void do_read_body(size_t length);

void do_read_header() {
	boost::asio::async_read(Globals::socket,
		boost::asio::buffer(Globals::currentHeader, HEADER_SIZE),
		[](boost::system::error_code ec, std::size_t /*length*/) {
		if (!ec) {
			int length = decode_header(Globals::currentHeader);
			do_read_body(length);
		}
		else {
			Globals::socketError = true;
			Globals::socket.close();
		}
	});
}

void do_read_body(size_t length) {
	char* body = new char[length];
	boost::asio::async_read(Globals::socket,
		boost::asio::buffer(body, length),
		[body, length](boost::system::error_code ec, std::size_t) {
		if (!ec) {
			protos::Message message;
			message.ParseFromArray(body, length);
			delete body;

			for (int i = 0; i < message.event_size(); i++) {
				auto& event = message.event(i);

				if (event.type() == event.ASSIGN) {
					Globals::ID = event.clientid();
				}
				if (event.type() == event.START_GAME) {
					Globals::startGame = true;
					Globals::gameWin = false;
				}
				if (event.type() == event.PLAYER_WIN) {
					Globals::gameWin = true;
				}
				if (event.type() == event.GAME_OVER) {
					Globals::gameOver = true;
				}
				if (event.type() == event.READY) {
					Globals::readyPlayers[event.clientid()] = true;
					Globals::gameWin = false;
				}
			}

			MessageHandler::gameLock.lock();
			MessageHandler::gameQueue.push_back(message);
			MessageHandler::gameLock.unlock();

			do_read_header();
		}
		else {
			Globals::socketError = true;
			Globals::socket.close();
		}
	});
}

int main(int argc, char *argv[]) {
	std::string hostname;
	std::string port;

	// load the settings from the config file
	if (!ConfigSettings::config->checkIfLoaded()) {
		if (!ConfigSettings::config->loadSettingsFile()) {
			std::cerr << "There was a problem loading the config file\n";
			return 1;
		}
	}
	// set the hostname and port
	if (!ConfigSettings::config->getValue(ConfigSettings::str_host_name, hostname)) {
		std::cerr << "There was a problem getting the hostname from the config file\n";
		return 1;
	}
	if (!ConfigSettings::config->getValue(ConfigSettings::str_port_number, port)) {
		std::cerr << "There was a problem getting the hostname from the config file\n";
		return 1;
	}

	// Create the GLFW window
	window = Window::create_window(1920, 1080);
	// Print OpenGL and GLSL versions
	print_versions();
	// Setup callbacks
	setup_callbacks();
	// Setup OpenGL settings, including lighting, materials, etc.
	setup_opengl_settings();
	glewExperimental = GL_TRUE;
	
	// Initialize GLEW to setup the OpenGL Function pointers
	GLenum glew_err = glewInit();
	if (glew_err != GLEW_OK) {
		fprintf(stderr, "GLEW NOT INITIALIZED");
		exit(-1);
	}

	tcp::resolver resolver(Globals::io_service);
	boost::asio::async_connect(Globals::socket, resolver.resolve({ hostname.c_str(), port.c_str() }),
		[](boost::system::error_code ec, tcp::resolver::iterator) {
		if (!ec) {
			do_read_header();
		}
	});

	std::thread t([]() {  Globals::io_service.run();  });

	while (Globals::ID == 0) {
		if (Globals::socketError) {
			Window::clean_up();
			glfwDestroyWindow(window);
			glfwTerminate();
			t.join();
			exit(EXIT_SUCCESS);
			return -1;
		}
		Sleep(1);
	}

	Globals::cam.setPlayerId(Globals::ID);

	bool joyStickPresent = glfwJoystickPresent(GLFW_JOYSTICK_1);

	if (!joyStickPresent) {
		cerr << "JOYSTICK NOT PRESENT" << endl;
	}
	

	Globals::soundEngine.initialize();

	// Initialize objects/pointers for rendering
	Window::initialize_objects();

	// Loop while GLFW window should stay open
	while (!glfwWindowShouldClose(window)) {
		if (joyStickPresent) {
			Window::handle_gamepad(window);
		}

		Globals::soundEngine.updateSystem();

		// Main render display callback. Rendering of objects is done here.
		Window::display_callback(window);
		// Idle callback. Updating objects, etc. can be done here.
		Window::idle_callback(window);
	}

	Window::clean_up();
	// Destroy the window
	glfwDestroyWindow(window);
	// Terminate GLFW
	glfwTerminate();

	Globals::socket.close();
	t.join();

	exit(EXIT_SUCCESS);
}