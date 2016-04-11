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
#include "Shader.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

GLuint Shader::currentlyBoundShaderID = 0x0;

Shader::Shader()
{
	init = false;
}

Shader::Shader(const char *vert, const char *frag, bool isFile)
{
	if (isFile)
	{
		//Read in the vertex and fragment shaders
		//We must delete these after we are finished compiling the shaders
		char* vv = read(vert);
		char* vf = read(frag);

		//Setup the shader
		setup(vv, vf);

		//Delete the file data arrays we allocted
		delete[] vv;
		delete[] vf;
	}
	else
	{
		//Treat the vert and frag parameters as shader code and directly compile them
		setup(vert, frag);
	}
	init = true;
}

Shader::~Shader()
{
	glDeleteProgram(pid);
}

void Shader::bind()
{
	if (currentlyBoundShaderID != pid)
	{
		currentlyBoundShaderID = pid;
		glUseProgram(pid);
	}
}

void Shader::unbind()
{
	if (currentlyBoundShaderID != (0x0))
	{
		currentlyBoundShaderID = (0x0);
		glUseProgram(0);
	}
}

GLuint Shader::getPid(){
	return currentlyBoundShaderID;
}

bool Shader::isInitilized(){
	return init;
}

void Shader::printLog(const char* tag)
{
	char glslLog[1024];
	GLsizei glslLogSize;

	//Extract the error log for this shader's pid
	glGetProgramInfoLog(pid, 1024, &glslLogSize, glslLog);

	//If the log isn't empty, print the contents
	if (glslLogSize > 0)
		std::cerr << tag << "(" << pid << ") -  Shader error log:" << std::endl << glslLog << std::endl;
	else
		std::cerr << tag << "(" << pid << ") -  Shaders compiled successfully!" << std::endl;
}

char* Shader::read(const char *filename)
{
	char* shaderFile = 0;

	//Open the file
	FILE* fp = fopen(filename, "rb");
	if (!fp)
	{
		std::cerr << "File doesn't exist [" << filename << "]" << std::endl;
		std::exit(-1);
	}

	//Obtain the file size
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	rewind(fp);

	//Alloc memory - will be deleted while setting the shader up
	shaderFile = new char[size + 1];

	//Copy the file to the shaderFile
	fread(shaderFile, sizeof(char), size, fp);
	shaderFile[size] = '\0'; //Eliminate the garbage at EOF
	fclose(fp);

	return shaderFile;
}

void Shader::setup(const char *vs, const char *fs)
{
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vs, NULL);

	glCompileShader(vertex);
	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// Similiar for Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fs, NULL);
	glCompileShader(fragment);
	// Print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// Shader Program
	this->pid = glCreateProgram();
	glAttachShader(this->pid, vertex);
	glAttachShader(this->pid, fragment);
	glLinkProgram(this->pid);
	// Print linking errors if any
	glGetProgramiv(this->pid, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->pid, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	//Unbind to not mix up shaders
	unbind();
}