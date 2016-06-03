#include "GameObjects.h"
#include "Globals.h"

const char* bearPath = "Assets/OBJ/Bear/bear_full.obj";
const char* wusonPath = "Assets/X/Wuson.X";
const char* mangoPath = "Assets/OBJ/Mango/mango.obj";
const char* chickenPath = "Assets/FBX/chicken_dance.fbx";
const char* cratePath = "Assets/OBJ/Crate/Crate1.obj";
const char* playerStandPath = "Assets/FBX/standing.fbx";
const char* playerRunPath = "Assets/FBX/running.fbx";
const char* playerPunchingPath = "Assets/FBX/punching.fbx";
const char* playerStunnedPath = "Assets/FBX/stun.fbx";
const char* wizardPath = "Assets/OBJ/Wizard_Hat/wizard_hat.obj";
const char* hardHatPath = "Assets/OBJ/Hard_Hat/HardHat.obj";
//const char* propellerHatPath = "Assets/OBJ/PropellerHat/PropellerHat.obj";
const char* propellerHatPath = "Assets/FBX/PropellerHat.fbx";
const char* bearHatPath = "Assets/OBJ/BearHat/BearHat.obj";
const char* deerstalkerHatPath = "Assets/OBJ/DeerstalkerHat/DeerstalkerHat.obj";

string skyboxDirectory = "Assets/Cubemap2";
string backgroundPath = "Assets/UI/DeerMenu.jpg";
string lobbyPath = "Assets/UI/Lobby.jpg";
string lobbyReadyPath = "Assets/UI/Lobby_Ready.jpg";
string endPath = "Assets/UI/EndGame.jpg";
string loadingPath = "Assets/UI/Loading.jpg";

GameObjects::GameObjects() {
}


GameObjects::~GameObjects() {
}

void GameObjects::loadShaders() {
	glm::vec3 pointLightPositions[] = {
		glm::vec3(2.3f, -1.6f, -3.0f),
		glm::vec3(-1.7f, 0.9f, 1.0f)
	};

	LightShader* lightShader = new LightShader(Globals::cam.getPosition(), "Shaders/shader_lighting.vert", "Shaders/shader_lighting.frag");
	lightShader->addDirectionalLight(glm::vec3(0.f, 1.0f, 0.f), glm::vec3(0.7f), glm::vec3(0.4f));
	//lightShader->addDirectionalLight(glm::vec3(0.2f, 10.0f, 0.f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f));
	//lightShader->addPointLight(pointLightPositions[0], glm::vec3(.8f), glm::vec3(1.0f), 1.0f, 0.009f, 0.0032f);
	//lightShader->addPointLight(pointLightPositions[1], glm::vec3(.8f), glm::vec3(1.0f), 1.0f, 0.009f, 0.0032f);

	LightShader* animShader = new LightShader(Globals::cam.getPosition(), "Shaders/shader_w_anim.vert", "Shaders/shader_w_anim.frag");
	animShader->addDirectionalLight(glm::vec3(0.f, 1.0f, 0.f), glm::vec3(0.7f), glm::vec3(0.4f));

	Shader *guiShader = new Shader("Shaders/guiItem.vert", "Shaders/guiItem.frag");
	Shader *twoShader = new Shader("Shaders/2DShader.vert", "Shaders/2DShader.frag");

	shaderMap[_LtShader] = lightShader;
	shaderMap[_GShader] = guiShader;
	shaderMap[_2DShader] = twoShader;
	shaderMap[_AShader] = animShader;

	ready = new ReadyGUI(Globals::drawData.width, Globals::drawData.height);
}

void GameObjects::loadLoadingObject() {
	//get this first so don't have white screen
	guiMap[_Loading] = new GuiItem(loadingPath, shaderMap[_GShader], 60, 34, 0, 0);
}


void GameObjects::loadGameObjects() {
	guiMap[_Background] = new GuiItem(backgroundPath, shaderMap[_GShader], 60, 34, 0, 0);
	guiMap[_LobbyBG] = new GuiItem(lobbyPath, shaderMap[_GShader], 60, 34, 0, 0);
	guiMap[_LobbyReadyBG] = new GuiItem(lobbyReadyPath, shaderMap[_GShader], 60, 34, 0, 0);
	guiMap[_EndGameBG] = new GuiItem(endPath, shaderMap[_GShader], 60, 34, 0, 0);

	numPlayerGuiSet = 0;

	playerGUIMap.push_back(std::pair<std::uint32_t, PlayerGameGUI*>(0, new PlayerGameGUI(shaderMap[_2DShader], Globals::drawData.width, Globals::drawData.height, "P1")));
	playerGUIMap.push_back(std::pair<std::uint32_t, PlayerGameGUI*>(0, new PlayerGameGUI(shaderMap[_2DShader], Globals::drawData.width, Globals::drawData.height, "P2")));
	playerGUIMap.push_back(std::pair<std::uint32_t, PlayerGameGUI*>(0, new PlayerGameGUI(shaderMap[_2DShader], Globals::drawData.width, Globals::drawData.height, "P3")));
	playerGUIMap.push_back(std::pair<std::uint32_t, PlayerGameGUI*>(0, new PlayerGameGUI(shaderMap[_2DShader], Globals::drawData.width, Globals::drawData.height, "P4")));

	loadModelMap();
	loadHatModelsMap();
	generateWorld(skyboxDirectory);
}

void GameObjects::loadModelMap() {
	modelMap[_Mango] = new Model(mangoPath, shaderMap[_AShader]);
	modelMap[_Player_Standing] = new PlayerModel(playerStandPath, shaderMap[_AShader], protos::Message_GameObject_AnimationState_STANDING);
	modelMap[_Player_Running] = new PlayerModel(playerRunPath, shaderMap[_AShader], protos::Message_GameObject_AnimationState_RUNNING);
	modelMap[_Player_Punching] = new PlayerModel(playerPunchingPath, shaderMap[_AShader], protos::Message_GameObject_AnimationState_PUNCHING);
	modelMap[_Player_Stunned] = new PlayerModel(playerStunnedPath, shaderMap[_AShader], protos::Message_GameObject_AnimationState_STUNNED);
	modelMap[_Player_Wuson] = new PlayerModel(wusonPath, shaderMap[_LtShader], protos::Message_GameObject_AnimationState_WUSON);
	modelMap[_Player_Bear] = new PlayerModel(bearPath, shaderMap[_LtShader], protos::Message_GameObject_AnimationState_BEAR);
	modelMap[_Player_Flying] = new PlayerModel(playerStandPath, shaderMap[_AShader], protos::Message_GameObject_AnimationState_FLYING);
	modelMap[_Crate] = new Model(cratePath, shaderMap[_AShader]);
	modelMap[_WizardHat] = new Model(wizardPath, shaderMap[_AShader]);
	modelMap[_HardHat] = new Model(hardHatPath, shaderMap[_AShader]);
	modelMap[_PropellerHat] = new AnimModel(propellerHatPath, shaderMap[_AShader]);
	modelMap[_BearHat] = new Model(bearHatPath, shaderMap[_AShader]);
	modelMap[_DeerstalkerHat] = new Model(deerstalkerHatPath, shaderMap[_AShader]);
}

void GameObjects::loadHatModelsMap() {
	Globals::hatModelsMap[WIZARD_HAT] = _WizardHat;
	Globals::hatModelsMap[HARD_HAT] = _HardHat;
	Globals::hatModelsMap[PROPELLER_HAT] = _PropellerHat;
	Globals::hatModelsMap[BEAR_HAT] = _BearHat;
	Globals::hatModelsMap[DEERSTALKER_HAT] = _DeerstalkerHat;
	Globals::hatModelsMap[CRATE] = _Crate;
}

void GameObjects::generateWorld(string directory) {
	World *world = new World();
	world->createWorld(shaderMap[_AShader], directory);
	cerr << "A" << endl;
	root = new SMatrixTransform();
	root->addNode(world);
	int width, height;
}

void GameObjects::setPlayerToGui(int id) {
	// Change first of pair to playerID
	if (numPlayerGuiSet < 4) {
		playerGUIMap[numPlayerGuiSet].first = id;
		numPlayerGuiSet++;
	}
}

void GameObjects::drawPlayerGui(glm::mat4 translation) {
	glm::vec3 red(1.0f, 0.0f, 0.0f);
	glm::vec3 yellow(0.6f, 0.2f, 1.0f);
	glm::mat4 mat = translation;
	for (int i = 0; i < playerGUIMap.size(); i++) {
		if (i > 0) {
			mat = glm::translate(mat, glm::vec3(500.0f, 0.0f, 0.0f));
		}
		glm::vec3 color;
		if (Globals::ID == playerGUIMap[i].first) {
			color = yellow;
		}
		else {
			color = red;
		}
		playerGUIMap[i].second->draw(mat, color);
	}
}

void GameObjects::updatePlayerGui(int id, int lives, int health) {
	for (int i = 0; i < numPlayerGuiSet; i++) {
		if (playerGUIMap[i].first == id) {
			playerGUIMap[i].second->update(lives, health);
		}
	}
}

void GameObjects::updatePlayerTime(float time, int id) {
	for (int i = 0; i < numPlayerGuiSet; i++) {
		if (playerGUIMap[i].first == id) {
			playerGUIMap[i].second->updateTime(time);
		}
	}
}