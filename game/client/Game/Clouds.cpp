#include "Clouds.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>  
#include <algorithm>

Clouds::Clouds(Shader *shader, float radius, int rings, int sectors, glm::vec3 center) : SGeode()
{
	this->shader = shader;
	this->radius = radius;
	this->rings = rings;
	this->sectors = sectors;
	this->center = center;
	setupCloud();
	setUpBuffer();
	setupUniformLoc();
	//offsets.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	//offsets.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	//offsets.push_back(glm::vec3(2.0f, 0.0f, 0.0f));
	//offsets.push_back(glm::vec3(3.0f, 0.0f, 0.0f));
	//offsets.push_back(glm::vec3(4.0f, 0.0f, 0.0f));
}
	

Clouds::~Clouds()
{
	
}

void Clouds::draw(DrawData& data) {

	float time = clock() / float(CLOCKS_PER_SEC);
	float delta = time - prevTime;
	prevTime = time;
	currCloudTime += min(delta, 0.05f);
	shader->bind();

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(data.matrix));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(data.view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(data.projection));

	glBindVertexArray(this->VAO);
	bindBuffers();
	//glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL, offsets.size());
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	shader->unbind();
}

void Clouds::update(UpdateData& data) {
	
}

void Clouds::setupCloud() {
	/*Create Sphere*/
	float const R = 1.f / (float)(rings - 1);
	float const S = 1.f / (float)(sectors - 1);
	int r, s;

	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++) {
			float const y = sin(-M_PI_2 + M_PI * r * R);
			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			vertices.push_back(glm::vec3(x, y, z));
			normals.push_back(glm::vec3(x, y, z));
			
		}
	}

	for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) {
		// create 2 triangles: 1 - 2 - 4  &  2 - 3 - 4
		indices.push_back(r * sectors + s); // 1
		indices.push_back(r * sectors + (s + 1)); // 2
		indices.push_back((r + 1) * sectors + s); // 4

		indices.push_back(r * sectors + (s + 1)); // 2
		indices.push_back((r + 1) * sectors + (s + 1)); // 3
		indices.push_back((r + 1) * sectors + s); // 4
	}
}

void Clouds::setUpBuffer() {
	// Create buffers/arrays
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->verticesBuffer);
	glGenBuffers(1, &this->normalsBuffer);
	//glGenBuffers(1, &this->offsetsBuffer);
	//glGenBuffers(1, &this->nonseBuffer);
	//glGenBuffers(1, &this->scaleBuffer);
	//glGenBuffers(1, &this->thresholdNormalsBuffer);
	//glGenBuffers(1, &this->frontPointsBuffer);
	//glGenBuffers(1, &this->backPointsBuffer);
	glGenBuffers(1, &this->EBO);
}

void Clouds::setupUniformLoc(){
	if (!shader->isInitilized()) {
		cerr << "Shader not initialized" << endl;
		exit(-1);
	}

	shader->bind();

	modelLoc = glGetUniformLocation(shader->getPid(), "model");
	viewLoc = glGetUniformLocation(shader->getPid(), "view");
	projLoc = glGetUniformLocation(shader->getPid(), "projection");

	shader->unbind();
}

void Clouds::bindBuffers() {
	 // Load data into vertex buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, this->verticesBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, this->normalsBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, this->offsetsBuffer);
	//glBufferData(GL_ARRAY_BUFFER, this->offsets.size() * sizeof(glm::vec3), &this->offsets[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, this->nonseBuffer);
	//glBufferData(GL_ARRAY_BUFFER, this->nonses.size() * sizeof(GLfloat), &this->nonses[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, this->scaleBuffer);
	//glBufferData(GL_ARRAY_BUFFER, this->scales.size() * sizeof(GLfloat), &this->scales[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, this->thresholdNormalsBuffer);
	//glBufferData(GL_ARRAY_BUFFER, this->thresholdNormals.size() * sizeof(glm::vec3), &this->thresholdNormals[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, this->frontPointsBuffer);
	//glBufferData(GL_ARRAY_BUFFER, this->frontPoints.size() * sizeof(glm::vec3), &this->frontPoints[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, this->backPointsBuffer);
	//glBufferData(GL_ARRAY_BUFFER, this->backPoints.size() * sizeof(glm::vec3), &this->backPoints[0], GL_STATIC_DRAW);

	// Position
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	// Normals
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	// Offset
	/*glEnableVertexAttribArray(OFFSET_LOCATION);
	glVertexAttribPointer(OFFSET_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glVertexAttribDivisor(OFFSET_LOCATION, 1);*/
	
}
