#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game\SMatrixTransform.h"
#include "Game\Model.h"
#include "GameObjects.h"


#include "util\Protos.pb.h"

class Window {
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
	static void handle_gamepad(GLFWwindow* window);
	static SMatrixTransform* createGameObj(Models modelType, Model* model, int id);
	static void cycleCamera();
	static void resetStates();
	//static void generateWorld(string directory);

private:
	static void addMoveEvent(protos::Message& message, protos::Event_Direction direction);
};
