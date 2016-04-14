#include "SGroup.h"


SGroup::SGroup() {
}

SGroup::~SGroup() {
}

void SGroup::draw(DrawData &drawData) {
	
	// Iterator to go through list of nodes and draw them
	list<SNode*>::iterator beg = listOfNodes->begin();
	
	for (; beg != listOfNodes->end(); beg++) {
		(*beg)->draw(drawData);
	}

}

void SGroup::update(DrawData &drawData) {

	list<SNode*>::iterator beg = listOfNodes->begin();

	for (; beg != listOfNodes->end(); beg++) {
		(*beg)->draw(drawData);
	}

}

void SGroup::addNode(SNode *node) {
	listOfNodes->push_back(node);
}

void SGroup::removeNode(SNode *node) {
	listOfNodes->remove(node);
}