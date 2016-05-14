#pragma once

#include "Model.h"

enum HatType{
	_none,
	_wizard,
	_mango
};

class Hat : public SGeode {
private:
	bool isVisible;
	HatType hatType;
	Model *model;

public:
	Hat(HatType hatType, Model *model);
	~Hat();

	virtual void update(UpdateData&);
	virtual void draw(DrawData&);
	void setVisible(bool visisible);

};