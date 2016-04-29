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
	LightShader* lightShader = new LightShader(Globals::camera.getPosition(), "Graphics/Shaders/shader_lighting.vert", "Graphics/Shaders/shader_lighting.frag");
	Model* model = new Model(floorPath, lightShader);
	lightShader->addDirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f));
	//lightShader->addDirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f));
	lightShader->addPointLight(pointLightPositions[0], glm::vec3(0.05f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.009f, 0.0032f);
	lightShader->addPointLight(pointLightPositions[1], glm::vec3(0.05f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.009f, 0.0032f);


	glm::mat4 mat = glm::mat4();//glm::scale(glm::vec3(50.0f, 0.0f, 50.0f));
	

	floor->setMatrix(glm::translate(mat, glm::vec3(0.0f, -6.0f, 0.0f)));

	floor->addNode(model);

	this->addNode(floor);
}
