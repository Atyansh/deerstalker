#include "Mango.h"

const char* modelPath = "Graphics/Assets/OBJ/Mango/mango.obj";
 //const char* modelPath = "Graphics/Assets/OBJ/Cube/Cube.obj";
 //const char* modelPath = "Graphics/Assets/FBX/pizza.fbx";
 //const char* modelPath = "Graphics/Assets/FBX/characterFBX.fbx";
 //const char* modelPath = "Graphics/Assets/FBX/chicken_dance.fbx";

Mango::Mango() {
}

Mango::~Mango() {
}

Mango* Mango::createNewMango() {
	glm::vec3 pointLightPositions[] = {
		glm::vec3(2.3f, -1.6f, -3.0f),
		glm::vec3(-1.7f, 0.9f, 1.0f)
	};
	LightShader* lightShader = new LightShader(Globals::camera.getPosition(), "Graphics/Shaders/shader_lighting.vert", "Graphics/Shaders/shader_lighting.frag");
	Model* model = new Model(modelPath, lightShader);
	lightShader->addDirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f));
	//lightShader->addDirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f));
	lightShader->addPointLight(pointLightPositions[0], glm::vec3(0.05f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.009f, 0.0032f);
	lightShader->addPointLight(pointLightPositions[1], glm::vec3(0.05f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.009f, 0.0032f);

	Mango* mango = new Mango();
	mango->setMatrix(glm::mat4());
	mango->addNode(model);

	return mango;
}