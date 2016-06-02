#pragma once

#include "Model.h"
#include "AnimModel.h"

class Hat : public SGeode {
private:
	bool isVisible;
	bool animate;
	Model *model;

public:
	Hat(Model *model, bool animate=false);
	~Hat();

	virtual void update(UpdateData&);
	virtual void draw(DrawData&);
	void setVisible(bool visible);
	bool getVisible() {
		return isVisible;
	}
};