#pragma once

#include "client\Globals.h"
#include "Model.h"
#include "LightShader.h"
#include "SMatrixTransform.h"

class Player : public SMatrixTransform {
public:
	Player();
	~Player();

	static Player* createNewPlayer();
	static char* modelPath;
};