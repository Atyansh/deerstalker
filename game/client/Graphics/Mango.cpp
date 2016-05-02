#include "Mango.h"

//const char* modelPath = "Graphics/Assets/OBJ/Mango/mango.obj";
 //const char* modelPath = "Graphics/Assets/OBJ/Cube/Cube.obj";
 const char* modelPath = "Graphics/Assets/FBX/HumanModelFight.fbx";
 //const char* modelPath = "Graphics/Assets/FBX/characterFBX.fbx";

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