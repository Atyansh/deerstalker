#pragma once

#include "Model.h"
#include "AnimModel.h"

class Hat : public SGeode {
private:
	bool isVisible;
	bool animate;
	Model *model;

	//Animation time
	float prevTime = 0;
	float currAnimTime = 0;

public:
	Hat(Model *model, bool animate=false);
	~Hat();

	virtual void update(UpdateData&);
	virtual void draw(DrawData&);
	void setVisible(bool visible);
	bool getVisible() {
		return isVisible;
	}
	void setAnimate(bool visible);
	bool getAnimate() {
		return animate;
	}
};