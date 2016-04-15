#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
private:
	static const int ENTER = 13;
	static const int ESC = 27;
	static const int SPACE = 32;

	static bool holdUp;
	static bool holdDown;
	static bool holdLeft;
	static bool holdRight;
	static bool holdLRot;
	static bool holdRRot;

	static int glfwToAscii(const int key);
	static int asciiToGLFW(const int key);
	static void calcMovements();

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
