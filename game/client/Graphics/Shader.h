/*
Copyright (C) 2006 So Yamaoka

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Modified: Timothy Kua(2016)

*/
#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*! Handles GLSL shaders.  It can load the code from a file or read straight
* from a char array. */
class Shader
{

private:

	const GLchar* read(const char *filename);
	void setup(const GLchar *vs, const GLchar *fs);

protected:
	bool init;
	GLuint pid;
	static GLuint currentlyBoundShaderID;

public:
	Shader();
	Shader(const char *vert, const char *frag, bool isFile = true);
	~Shader();

	void bind();
	void unbind();

	bool isInitilized();
	GLuint &getPid();

	void printLog(const char* tag = "");

};