#pragma once

#include "client/Graphics/DrawData.h"
#include "client/Graphics/UpdateData.h"

class SNode {

public:

	SNode();
	~SNode();

	virtual void draw(DrawData &data) = 0;
	virtual void update(UpdateData &data) = 0;

};

