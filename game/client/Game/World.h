#pragma once

#include <list>
#include <string>
#include <glm/glm.hpp>
#include "client\Graphics\SMatrixTransform.h"
#include "client\Graphics\Shader.h"
#include "Skybox.h"
using namespace std;

class World : public SMatrixTransform {

protected:
	SMatrixTransform *floor;
	Skybox skybox;

public:
	World();
	~World();

	void createWorld(Shader *shader, string directory);
	void draw(DrawData &data);


};

