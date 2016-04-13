#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Shader
{

private:
	void setup(const char *vs, const char *fs, bool isFile);

protected:
	bool init;
	GLuint pid;
	static GLuint currentlyBoundShaderID;

public:
	Shader();
	Shader(const char *vert, const char *frag, bool isFile = true);
	~Shader();

	virtual void bind();
	virtual void unbind();

	bool isInitilized();
	GLuint &getPid();

	void printLog(const char* tag = "");

};