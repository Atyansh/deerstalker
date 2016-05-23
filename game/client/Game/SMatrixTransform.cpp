#include "SMatrixTransform.h"


SMatrixTransform::SMatrixTransform() {
	this->drawData.matrix = glm::mat4();	// Set to identity matrix
}

SMatrixTransform::SMatrixTransform(DrawData &data) {
	this->drawData = data;
}

SMatrixTransform::~SMatrixTransform() {
}

void SMatrixTransform::draw(DrawData &data) {

	DrawData newData = data;
	newData.matrix = data.matrix * this->drawData.matrix;
	
	list<SNode*>::iterator beg = listOfNodes->begin();
	
	// For each child of the node, draw with new data
	for (; beg != listOfNodes->end(); beg++) {
		(*beg)->draw(newData);
	}
}

void SMatrixTransform::update(UpdateData &data) {
	
	list<SNode*>::iterator beg = listOfNodes->begin();

	for (; beg != listOfNodes->end(); beg++) {
		(*beg)->update(data);
	}
}

void SMatrixTransform::setMatrix(glm::mat4 mat) {
	drawData.matrix = mat;
}

DrawData SMatrixTransform::getDrawData() {
	return this->drawData;
}