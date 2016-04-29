#include "SGroup.h"
#include <iostream>


#define deleteList(__list__) do {\
	iter = __list__->begin(); \
	end = __list__->end(); \
while (iter != end) delete (*(iter++)); \
	delete __list__; \
} while (false)

SGroup::SGroup() {
	listOfNodes = new list<SNode*>();
}

SGroup::~SGroup() {
	std::list<SNode*>::iterator iter;
	std::list<SNode*>::iterator end;

	deleteList(listOfNodes);
}

void SGroup::draw(DrawData &data) {
	
	// Iterator to go through list of nodes and draw them
	list<SNode*>::iterator beg = listOfNodes->begin();
	
	for (; beg != listOfNodes->end(); beg++) {
		(*beg)->draw(data);
	}

}

void SGroup::update(UpdateData &data) {

	list<SNode*>::iterator beg = listOfNodes->begin();

	for (; beg != listOfNodes->end(); beg++) {
		(*beg)->update(data);
	}

}

void SGroup::addNode(SNode *node) {
	listOfNodes->push_back(node);
}

void SGroup::removeNode(SNode *node) {
	listOfNodes->remove(node);
}