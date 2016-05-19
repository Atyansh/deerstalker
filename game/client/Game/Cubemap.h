#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <assimp/scene.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Cubemap {

private:

	static unsigned char* loadPPM(const char* filename, int &width, int &height);

public:

	Cubemap();
	~Cubemap();


	static GLuint loadCubemap(const string& directory);
	

};

