#include "FontShader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


FontShader::FontShader() {}

FontShader::FontShader(int width, int height, const char *vs, const char *fg, bool isFile) : Shader(vs, fg, isFile) {
	_width = width;
	_height = height;

	//setupProjection();
}

FontShader::~FontShader() {}
//
//void FontShader::setupProjection() {
//	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(_width), 0.0f, static_cast<GLfloat>(_height));
//	bind();
//	glUniformMatrix4fv(glGetUniformLocation(pid, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
//}
//
//void FontShader::updateProjection(int width, int height) {
//	_width = width;
//	_height = height;
//
//	setupProjection();
//}

void FontShader::bind() {
	if (currentlyBoundShaderID != pid)
	{
		currentlyBoundShaderID = pid;
		glUseProgram(pid);
	}
}

void FontShader::unbind() {
	if (currentlyBoundShaderID != (0x0))
	{
		currentlyBoundShaderID = (0x0);
		glUseProgram(0);
	}
}