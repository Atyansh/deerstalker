#include "Mango.h"

//const char* modelPath = "Graphics/Assets/OBJ/Player/Player.obj";
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
	Model* model = new Model(modelPath, Globals::lightShader);
	
	Mango* mango = new Mango();
	mango->setMatrix(glm::mat4());
	mango->addNode(model);

	return mango;
}