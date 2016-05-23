#pragma once

#include <glm/glm.hpp>

#include "SGroup.h"
#include "client/Graphics/DrawData.h"
#include "client/Graphics/UpdateData.h"

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

