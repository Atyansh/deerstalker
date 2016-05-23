#pragma once

#include "SNode.h"
#include "client/Graphics/UpdateData.h"

class SGeode : public SNode {

public:
	SGeode();
	~SGeode();

	void draw(DrawData &data);
	void update(UpdateData &data);

};

