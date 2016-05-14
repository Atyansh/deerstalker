#pragma once

#include "Model.h"

enum HatType{
	_none,
	_wizard
};

class Hat : public Model {
private:
	bool isVisible;
	HatType hatType;

public:
	Hat(HatType hatType, const char* path, Shader *shader);
	~Hat();

	virtual void update(UpdateData&);
	virtual void draw(DrawData&);
	void setVisible(bool visisible);

};