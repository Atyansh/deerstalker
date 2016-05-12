#include "World.h"
#include "Model.h"
#include "client\Globals.h"
#include <glm/gtx/transform.hpp>

const char* floorPath = "Graphics/Assets/OBJ/Floor/Floor.obj";

World::World() {
	//this->drawData.matrix = glm::mat4();
}


World::~World() {

}

void World::createWorld(Shader* shader) {
	floor = new SMatrixTransform();
	
	glm::vec3 pointLightPositions[] = {
		glm::vec3(2.3f, -1.6f, -3.0f),
		glm::vec3(-1.7f, 0.9f, 1.0f)
	};
	
	Model* model = new Model(floorPath, shader);

	glm::mat4 mat = glm::mat4();
	
	glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(10.f, 1.f, 10.f));
	

	floor->setMatrix(glm::translate(mat * scale, glm::vec3(0.0f, -6.0f, 0.0f)));

	floor->addNode(model);

	this->addNode(floor);
}
