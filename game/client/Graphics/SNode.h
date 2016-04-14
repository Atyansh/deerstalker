#pragma once

#include "DrawData.h"
#include "UpdateData.h"

class SNode {

public:

	SNode();
	~SNode();

	virtual void draw(DrawData &data) = 0;
	virtual void update(UpdateData &data) = 0;

};

