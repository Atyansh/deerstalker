#pragma once

#include <list>
#include "SNode.h"
#include "UpdateData.h"

using namespace std;

class SGroup : public SNode {

protected:

	// Store nodes
	list<SNode*>* listOfNodes; 

public:
	SGroup();
	~SGroup();

	void draw(DrawData &data);
	void update(UpdateData &data);

	void addNode(SNode *node);
	void removeNode(SNode *node);

};
 
