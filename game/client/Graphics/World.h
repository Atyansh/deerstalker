#pragma once

#include <list>
#include <glm/glm.hpp>
#include "SMatrixTransform.h"

using namespace std;

class World : public SMatrixTransform {

protected:
	SMatrixTransform *floor;

public:
	World();
	~World();

	void createWorld();


};

