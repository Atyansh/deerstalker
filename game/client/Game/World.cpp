#include "World.h"
#include "Model.h"
#include "client\Globals.h"
#include "Clouds.h"
#include <glm/gtx/transform.hpp>

const char* floorPath = "Assets/OBJ/Floor/Floor.obj";
const char* constructionStagePath = "Assets/OBJ/Construction_Stage_OBJ_Updated/Construction_Stage_OBJ_Scaled_Separated.obj";
const char* wreckingBallBasePath = "Assets/OBJ/Construction_Stage_OBJ_Updated/WreckingBallBase.obj";
const char* wreckingBallPath = "Assets/OBJ/Construction_Stage_OBJ_Updated/WreckingBall.obj";
const char* billboardBallPath = "Assets/OBJ/Construction_Stage_OBJ_Updated/BillboardPlatform.obj";

World::World() {
	//this->drawData.matrix = glm::mat4();
}


World::~World() {

}

void World::createWorld(Shader* shader, string directory) {
	floor = new SMatrixTransform();
	
	glm::vec3 pointLightPositions[] = {
		glm::vec3(2.3f, -1.6f, -3.0f),
		glm::vec3(-1.7f, 0.9f, 1.0f)
	};
	
	Model* model = new Model(constructionStagePath, shader);
	Model* wreckingBallBase = new Model(wreckingBallBasePath, shader);
	Model* wreckingBall = new Model(wreckingBallPath, shader);
	Model* billboard = new Model(billboardBallPath, shader);

	glm::mat4 mat = glm::mat4();
	
	//glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(10.f, 1.f, 10.f));
	

	floor->setMatrix(mat);

	floor->addNode(model);
	floor->addNode(wreckingBall);
	floor->addNode(wreckingBallBase);
	floor->addNode(billboard);

	Shader *cloudShader = new Shader("Shaders/cloud.vert", "Shaders/cloud.frag");
	floor->addNode(new Clouds(cloudShader, 1, 25, 25));

	this->addNode(floor);

	skybox.setupVAO(directory);
}

void World::draw(DrawData &data) {
	SMatrixTransform::draw(data);
	skybox.draw();
}