#pragma once

#include "DrawData.h"

class SNode {

public:

	SNode();
	~SNode();

	virtual void draw(DrawData &drawData) = 0;
	virtual void update(DrawData &drawData) = 0;

};

