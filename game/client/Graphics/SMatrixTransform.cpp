#include "SMatrixTransform.h"


SMatrixTransform::SMatrixTransform() {
}

SMatrixTransform::~SMatrixTransform() {
}

void SMatrixTransform::draw(DrawData &drawData) {
	
}

void SMatrixTransform::update(DrawData &drawData) {
	
}

glm::mat4 SMatrixTransform::getMatrix() {
	return drawData.localMat;
}

void SMatrixTransform::setMatrix(glm::mat4 mat) {
	drawData.localMat = mat;
}