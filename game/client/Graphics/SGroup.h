#pragma once

#include <list>
#include "SNode.h"

using namespace std;

class SGroup : public SNode {

protected:

	// Store nodes
	list<SNode*>* listOfNodes; 

public:
	SGroup();
	~SGroup();

	void draw(DrawData &drawData);
	void update(DrawData &drawData);

	void addNode(SNode *node);
	void removeNode(SNode *node);

};
 
