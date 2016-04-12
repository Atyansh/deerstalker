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
#include <fstream>
#include <sstream>

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
		//const GLchar* vv = read(vert);
		//const GLchar* vf = read(frag);

		//Setup the shader
		setup(vert, frag);

		//Delete the file data arrays we allocted
		//delete[] vv;
		//delete[] vf;
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
		fprintf(stderr, "Binding shader now\n");
		fprintf(stderr, "PID: %u\n", pid);
		fprintf(stderr, "CurrentlyBoundShaderID: %u\n", currentlyBoundShaderID);
		glUseProgram(pid);
	}
}

void Shader::unbind()
{
	if (currentlyBoundShaderID != (0x0))
	{
		currentlyBoundShaderID = (0x0);
		glUseProgram(0);

		fprintf(stderr, "Unbinding shaders\n");
	}
}

GLuint &Shader::getPid(){
	return currentlyBoundShaderID;

	//return this->pid;
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

const GLchar* Shader::read(const char *filename)
{
	std::string shaderCode;
	std::ifstream shaderFile;
	// ensures ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::badbit);
	try
	{
		// Open files
		shaderFile.open(filename);
		std::stringstream shaderStream;
		// Read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();
		// close file handlers
		shaderFile.close();
		// Convert stream into string
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	std::cout << filename << "\n" << shaderCode << std::endl;
	return shaderCode.c_str();


}

void Shader::setup(const GLchar *vs, const GLchar *fs)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensures ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try
	{
		// Open files
		vShaderFile.open(vs);
		fShaderFile.open(fs);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar * fShaderCode = fragmentCode.c_str();
	// 2. Compile shaders
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];
	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// Print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// Print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Shader Program
	this->pid = glCreateProgram();

	fprintf(stderr, "PID during setup after glCreateProgram: %u\n", this->pid);

	glAttachShader(this->pid, vertex);
	glAttachShader(this->pid, fragment);
	std::cout << vertex << " " << fragment << std::endl;
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
	//unbind();
}