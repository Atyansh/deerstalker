#include "World.h"
#include "LightShader.h"
#include "Model.h"
#include "client\Globals.h"
#include <glm/gtx/transform.hpp>
#include "Mango.h"

const char* floorPath = "Graphics/Assets/OBJ/Floor/Floor.obj";

World::World() {
	//this->drawData.matrix = glm::mat4();
	this->createWorld();
}


World::~World() {

}

void World::createWorld() {
	floor = new SMatrixTransform();
	
	glm::vec3 pointLightPositions[] = {
		glm::vec3(2.3f, -1.6f, -3.0f),
		glm::vec3(-1.7f, 0.9f, 1.0f)
	};
	
	Model* model = new Model(floorPath, Globals::lightShader);

	glm::mat4 mat = glm::mat4();
	
	glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(100.f, 1.f, 100.f));
	

	floor->setMatrix(glm::translate(mat * scale, glm::vec3(0.0f, -6.0f, 0.0f)));

	floor->addNode(model);

	this->addNode(floor);
}
