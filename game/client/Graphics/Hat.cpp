#include "Hat.h"

Hat::Hat(HatType hatType, const char* path, Shader *shader) : Model(path, shader)
{
	isVisible = false;
	this->hatType = hatType;
}
	

Hat::~Hat()
{
	
}

void Hat::draw(DrawData& data) {
	if (isVisible) {
		Model::draw(data);
	}
}

void Hat::update(UpdateData& data) {
	if (isVisible) {
		Model::update(data);
	}
}

void Hat::setVisible(bool visible) {
	this->isVisible = visible;
}