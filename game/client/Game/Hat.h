#pragma once

#include "client\Graphics\Model.h"

enum HatType{
	_none,
	_wizard,
	_crate
};

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

};