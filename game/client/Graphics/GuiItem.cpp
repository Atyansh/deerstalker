#include "GuiItem.h"

#include <SOIL\SOIL.h>
#include <glm/ext.hpp>

GuiItem::GuiItem(string filename, Shader *shader, int w=100, int h=100, int x=0, int y=0): Drawable(){
	this->filename = filename;
	this->shader = shader;
	xPos = x;
	yPos = y;
	init(w, h);
}

GuiItem::~GuiItem(){

}

void GuiItem::init(int width, int height){
	GLfloat vertices[] = {
		// Positions 
		 width / 2.f,  height / 2.f, 0.f, 1.0f, 1.0f,
		 width / 2.f, -height / 2.f, 0.f, 1.0f, 0.0f,
		-width / 2.f, -height / 2.f, 0.f, 0.0f, 0.0f,
		-width / 2.f,  height / 2.f, 0.f, 0.0f, 1.0f
		 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO


	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	if (image == NULL) {
		this->textureID = -1;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	this->textureID = textureID;

}

void GuiItem::setPosition(int x, int y){
	xPos = x;
	yPos = y;
}

void GuiItem::translate(int x, int y){
	xPos += x;
	yPos += y;
}

int GuiItem::getXPos(){
	return xPos;
}

int GuiItem::getYPos(){
	return yPos;
}

void GuiItem::draw(DrawData &data){
	shader->bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(glGetUniformLocation(shader->getPid(), "guiItem"), 0);
	glUniformMatrix4fv(glGetUniformLocation(shader->getPid(), "view"), 1, GL_FALSE, glm::value_ptr(data.view));
	glUniformMatrix4fv(glGetUniformLocation(shader->getPid(), "projection"), 1, GL_FALSE, glm::value_ptr(data.projection));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


	shader->unbind();
}

void GuiItem::update(UpdateData &data){

}

