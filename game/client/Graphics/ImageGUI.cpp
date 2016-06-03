#include "ImageGUI.h"

#include <glm\ext.hpp>

ImageGUI::ImageGUI(const char* imagePath, Shader* shader, int w, int h) {
	_path = imagePath;
	_projection = glm::ortho(0.0f, static_cast<GLfloat>(w), 0.0f, static_cast<GLfloat>(h));
	_shader = shader;
	width = w;
	height = h;
}


ImageGUI::~ImageGUI() {
}

void ImageGUI::setupBuffers() {

	GLfloat vertices[] = {
		// Pos      // Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	unsigned char* image = SOIL_load_image(_path, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == NULL) {
		fprintf(stderr, "I AM NULL\n");
		this->textureID = -1;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	this->textureID = textureID;
	fprintf(stderr, "Texture ID: %d\n", this->textureID);
}

void ImageGUI::draw() {
	fprintf(stderr, "I AM DRAWING\n");
	_shader->bind();
	glm::mat4 model;
	/*
	model = glm::translate(model, glm::vec3(position, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	this->shader.SetMatrix4("model", model);
	*/

	glUniformMatrix4fv(glGetUniformLocation(_shader->getPid(), "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(_shader->getPid(), "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	_shader->unbind();

	/*_shader->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(glGetUniformLocation(_shader->getPid(), "guiItem"), 0);
	glm::mat4 mat;
	glUniformMatrix4fv(glGetUniformLocation(_shader->getPid(), "view"), 1, GL_FALSE, glm::value_ptr(mat));
	glUniformMatrix4fv(glGetUniformLocation(_shader->getPid(), "projection"), 1, GL_FALSE, glm::value_ptr(_projection));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


	_shader->unbind();*/
}