#pragma once

#include <glm/glm.hpp>

#include "SGroup.h"
#include "DrawData.h"

class SMatrixTransform : public SGroup {

private:
	DrawData drawData;

public:

	SMatrixTransform();
	~SMatrixTransform();

	void draw(DrawData &drawData);
	void update(DrawData &drawData);

	glm::mat4 getMatrix();
	void setMatrix(glm::mat4 mat);
};

