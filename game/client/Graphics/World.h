#pragma once

#include <list>
#include <glm/glm.hpp>
#include "SMatrixTransform.h"
#include "Shader.h"

using namespace std;

class World : public SMatrixTransform {

protected:
	SMatrixTransform *floor;

public:
	World();
	~World();

	void createWorld(Shader *shader);


};

