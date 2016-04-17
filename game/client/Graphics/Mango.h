#pragma once

#include "Globals.h"
#include "Model.h"
#include "LightShader.h"
#include "SMatrixTransform.h"

class Mango : public SMatrixTransform {
public:
	Mango();
	~Mango();

	static Mango* createNewMango();
};