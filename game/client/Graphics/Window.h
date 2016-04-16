#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Cube.h"

class Window {
private:
	static const int ENTER = 13;
	static const int ESC = 27;
	static const int SPACE = 32;

	static void calcMovements(Cube& cube);

public:
	static int width;
	static int height;
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback(GLFWwindow* window);
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
