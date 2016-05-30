#include "LivesGUI.h"

#include <GL\glew.h>
#include <GLFW/glfw3.h>


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


LivesGUI::LivesGUI()
{
}


LivesGUI::~LivesGUI()
{
}

void LivesGUI::setupBuffers() {
	// Base of healthbar. Static unchanging
	GLfloat base_vertices[] = {

		// Pos    
		0.0f, 100.0f, 0.0f, 1.0f, 1.0f,
		100.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f,

		0.0f, 100.0f, 0.0f, 1.0f, 1.0f,
		100.0f, 100.0f, 0.0f, 1.0f, 1.0f,
		100.0f, 0.0f, 0.0f, 1.0f, 1.0f


	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(base_vertices), base_vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}


void LivesGUI::draw(glm::mat4 translation) {

	shader->bind();


	glUniformMatrix4fv(glGetUniformLocation(shader->getPid(), "projection"), 1, GL_FALSE, glm::value_ptr(_projection));
	glm::mat4 mat = translation;
	glUniformMatrix4fv(glGetUniformLocation(shader->getPid(), "model"), 1, GL_FALSE, glm::value_ptr(mat));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);


}

void LivesGUI::update() {
	setupBuffers();
}