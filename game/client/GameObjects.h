#pragma once

#include <unordered_map>
#include "Graphics\LightShader.h"
#include "Graphics\Model.h"
#include "Graphics\SNode.h"
#include "Graphics\SMatrixTransform.h"
#include "Game\Player.h"
#include "Game\World.h"

enum Models {
	_Player,
	_Mango,
	_Crate,
	_Wizard
};

enum Shaders {
	_BShader,
	_LtShader
};

class GameObjects
{
private:
	void generateWorld(string directory);
public:

	std::unordered_map<std::uint32_t, SMatrixTransform*> playerMap;
	std::unordered_map<std::uint32_t, SMatrixTransform*> hatMap;
	std::unordered_map<std::uint32_t, SMatrixTransform*> bulletMap;
	std::unordered_map<std::uint32_t, Model*> modelMap;
	std::unordered_map<std::uint32_t, Shader*> shaderMap;
	SMatrixTransform *root;

	GameObjects();
	~GameObjects();

	void loadGameObjects();
};

