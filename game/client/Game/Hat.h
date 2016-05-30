#pragma once

#include "Model.h"

class Hat : public SGeode {
private:
	bool isVisible;
	Model *model;

public:
	Hat(Model *model);
	~Hat();

	virtual void update(UpdateData&);
	virtual void draw(DrawData&);
	void setVisible(bool visible);
	bool getVisible() {
		return isVisible;
	}
};