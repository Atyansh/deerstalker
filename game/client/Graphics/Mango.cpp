#include "Mango.h"

char* Mango::modelPath = "Graphics/Assets/OBJ/Mango/mango.obj";

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