#pragma once

#include "SNode.h"

class SGeode : public SNode {

public:
	SGeode();
	~SGeode();

	void draw(DrawData &drawData);
	void update(DrawData &drawData);

};

