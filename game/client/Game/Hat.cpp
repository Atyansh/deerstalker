#include "Hat.h"
#include <ctime>

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
		
		if (animate) {
			float time = clock() / float(CLOCKS_PER_SEC);
			float delta = time - prevTime;
			prevTime = time;

			currAnimTime += min(delta, 0.05f);
			data.animTime = currAnimTime;
			dynamic_cast<AnimModel*>(model)->setAnimation(true);
			model->draw(data);
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