#pragma once

#include <unordered_map>
#include <vector>
#include "Graphics\LightShader.h"
#include "Game\Model.h"
#include "Game\SNode.h"
#include "Game\SMatrixTransform.h"
#include "Game\Player.h"
#include "Game\World.h"
#include "Graphics\GuiItem.h"
#include "Graphics\PlayerGameGUI.h"

enum Models {
	_Player, //used for type of object
	_Player_Standing,
	_Player_Running,
	_Player_Punching,
	_Player_Stunned,
	_Player_Wuson,
	_Player_Bear,
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
	_LtShader,
	_2DShader,
	_AShader
};

enum GUI {
	_Background,
	_LobbyBG,
	_LobbyReadyBG,
	_EndGameBG,
	_Loading
};
class GameObjects
{
private:
	int numPlayerGuiSet;

	void generateWorld(string directory);
public:

	std::unordered_map<std::uint32_t, SMatrixTransform*> playerMap;
	std::vector<std::pair<std::uint32_t, PlayerGameGUI*>> playerGUIMap;
	std::unordered_map<std::uint32_t, SMatrixTransform*> hatMap;
	std::unordered_map<std::uint32_t, SMatrixTransform*> bulletMap;
	std::unordered_map<std::uint32_t, Model*> modelMap;
	std::unordered_map<std::uint32_t, Shader*> shaderMap;
	std::unordered_map<std::uint32_t, GuiItem*> guiMap;
	SMatrixTransform *root;

	GameObjects();
	~GameObjects();

	void loadShaders();
	void loadLoadingObject();
	void loadGameObjects();
	void loadModelMap();
	void loadHatModelsMap();
	void setPlayerToGui(int id);
	void drawPlayerGui(glm::mat4 translation);
	void updatePlayerGui(int id, int lives, int health);
};

