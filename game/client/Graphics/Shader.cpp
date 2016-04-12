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
	setup(vert, frag, isFile);
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

GLuint &Shader::getPid(){
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

void Shader::setup(const GLchar *vs, const GLchar *fs, bool isFile)
{
	std::string vertexCode;
	std::string fragmentCode;
	if (isFile){
		// 1. Retrieve the vertex/fragment source code from filePath
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
		catch (std::ifstream::failure e){
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
	}
	else{
		vertexCode = vs;
		fragmentCode = fs;
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
	//unbind();
}