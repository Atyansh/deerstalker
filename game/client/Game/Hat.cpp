#include "Hat.h"

Hat::Hat(Model *model, bool animate) : SGeode()
{
	isVisible = false;
	this->animate = animate;
	this->model = model;
}
	

Hat::~Hat()
{
	
}

void Hat::draw(DrawData& data) {
	if (isVisible) {
		cout << animate << endl;
		if (animate) {
			data.animate = animate;
			dynamic_cast<AnimModel*>(model)->draw(data);
		} else {
			model->draw(data);
		}
		
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

void Hat::setAnimate(bool animate) {
	this->animate = animate;
}