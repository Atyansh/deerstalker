#pragma once

#include <unordered_map>
#include "Graphics\LightShader.h"
#include "Game\Model.h"
#include "Game\SNode.h"
#include "Game\SMatrixTransform.h"
#include "Game\Player.h"
#include "Game\World.h"
#include "Graphics\GuiItem.h"

enum Models {
	_Player, //used for type of object
	_Player_Standing,
	_Player_Running,
	_Player_Punching,
	_Mango,
	_Crate,
	_WizardHat,
	_HardHat,
	_BearHat,
	_PropellerHat,
	_DeerstalkerHat
};

enum Shaders {
	_GShader,
	_LtShader
};

enum GUI {
	_Background,
	_LobbyBG,
	_EndGameBG
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
	std::unordered_map<std::uint32_t, GuiItem*> guiMap;
	SMatrixTransform *root;

	GameObjects();
	~GameObjects();

	void loadGameObjects();
	void loadModelMap();
	void loadHatModelsMap();
};

