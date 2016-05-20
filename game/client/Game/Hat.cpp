#include "Hat.h"

Hat::Hat(HatType hatType, Model *model) : SGeode()
{
	isVisible = false;
	this->hatType = hatType;
	this->model = model;
}
	

Hat::~Hat()
{
	
}

void Hat::draw(DrawData& data) {
	if (isVisible) {
		model->draw(data);
	}
}

void Hat::update(UpdateData& data) {
	if (isVisible) {
		model->update(data);
	}
}

void Hat::setVisible(bool visible) {
	this->isVisible = visible;
}