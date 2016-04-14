#pragma once

#include <glm/glm.hpp>

#include "SGroup.h"
#include "DrawData.h"
#include "UpdateData.h"

class SMatrixTransform : public SGroup {
protected:
	DrawData drawData;

public:

	SMatrixTransform();
	SMatrixTransform(DrawData &data);
	~SMatrixTransform();

	void draw(DrawData &data);
	void update(UpdateData &data);

	void setMatrix(glm::mat4 mat);
	DrawData getDrawData();

};

