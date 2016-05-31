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
const char* propellerHatPath = "Assets/OBJ/PropellerHat/PropellerHat.obj";
const char* bearHatPath = "Assets/OBJ/BearHat/BearHat.obj";
const char* deerstalkerHatPath = "Assets/OBJ/DeerstalkerHat/DeerstalkerHat.obj";

string skyboxDirectory = "Assets/Cubemap";
string backgroundPath = "Assets/UI/DeerMenu.jpg";
string lobbyPath = "Assets/UI/Lobby.jpg";
string endPath = "Assets/UI/EndGame.jpg";
string loadingPath = "Assets/UI/Loading.png";

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

	shaderMap[_LtShader] = lightShader;
	shaderMap[_GShader] = guiShader;
	shaderMap[_AShader] = animShader;
}

void GameObjects::loadLoadingObject() {
	//get this first so don't have white screen
	guiMap[_Loading] = new GuiItem(loadingPath, shaderMap[_GShader], 60, 34, 0, 0);
}


void GameObjects::loadGameObjects() {
	guiMap[_Background] = new GuiItem(backgroundPath, shaderMap[_GShader], 60, 34, 0, 0);
	guiMap[_LobbyBG] = new GuiItem(lobbyPath, shaderMap[_GShader], 60, 34, 0, 0);
	guiMap[_EndGameBG] = new GuiItem(endPath, shaderMap[_GShader], 60, 34, 0, 0);

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
	modelMap[_Crate] = new Model(cratePath, shaderMap[_AShader]);
	modelMap[_WizardHat] = new Model(wizardPath, shaderMap[_AShader]);
	modelMap[_HardHat] = new Model(hardHatPath, shaderMap[_AShader]);
	modelMap[_PropellerHat] = new Model(propellerHatPath, shaderMap[_AShader]);
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