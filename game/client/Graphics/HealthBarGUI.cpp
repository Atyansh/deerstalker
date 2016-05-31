#include "HealthBarGUI.h"
#include <GL\glew.h>
#include <GLFW/glfw3.h>


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


HealthBarGUI::~HealthBarGUI() {
}

void HealthBarGUI::setupBuffers() {
	// Base of healthbar. Static unchanging
	GLfloat base_vertices[] = {

		20.0f, 80.0f, 1.0f, 1.0f, 0.0f,
		20.0f + (360.0f * _health), 20.0f, 1.0f, 1.0f, 0.0f,
		20.0f, 20.f, 1.0f, 1.0f, 0.0f,

		20.0f, 80.0f, 1.0f, 1.0f, 0.0f,
		20.0f + (360.0f * _health), 80.0f, 1.0f, 1.0f, 0.0f,
		20.0f + (360.0f * _health), 20.0f, 1.0f, 1.0f, 0.0f,

		// Pos    
		0.0f, 100.0f, 0.0f, 1.0f, 0.0f,
		400.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.0f, 100.0f, 0.0f, 1.0f, 0.0f,
		400.0f, 100.0f, 0.0f, 1.0f, 0.0f,
		400.0f, 0.0f, 0.0f, 1.0f, 0.0f
	
		
	};

	glGenVertexArrays(1, &baseVAO);
	glGenBuffers(1, &baseVBO);

	glBindBuffer(GL_ARRAY_BUFFER, baseVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(base_vertices), base_vertices, GL_STREAM_DRAW);

	glBindVertexArray(baseVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}


void HealthBarGUI::draw(glm::mat4 translation) {

	shader->bind();

	glUniformMatrix4fv(glGetUniformLocation(shader->getPid(), "projection"), 1, GL_FALSE, glm::value_ptr(_projection));
	glm::mat4 mat = translation;
	glUniformMatrix4fv(glGetUniformLocation(shader->getPid(), "model"), 1, GL_FALSE, glm::value_ptr(mat));

	glBindVertexArray(baseVAO);
	glDrawArrays(GL_TRIANGLES, 0, 12);
	glBindVertexArray(0);

	
}

void HealthBarGUI::update(float health) {
	_health = health;
	//setupBuffers();

	GLfloat base_vertices[] = {

		20.0f, 80.0f, 1.0f, 1.0f, 0.0f,
		20.0f + (360.0f * _health), 20.0f, 1.0f, 1.0f, 0.0f,
		20.0f, 20.f, 1.0f, 1.0f, 0.0f,

		20.0f, 80.0f, 1.0f, 1.0f, 0.0f,
		20.0f + (360.0f * _health), 80.0f, 1.0f, 1.0f, 0.0f,
		20.0f + (360.0f * _health), 20.0f, 1.0f, 1.0f, 0.0f,

		// Pos    
		0.0f, 100.0f, 0.0f, 1.0f, 0.0f,
		400.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.0f, 100.0f, 0.0f, 1.0f, 0.0f,
		400.0f, 100.0f, 0.0f, 1.0f, 0.0f,
		400.0f, 0.0f, 0.0f, 1.0f, 0.0f


	};



	glBindBuffer(GL_ARRAY_BUFFER, baseVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(base_vertices), base_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}
