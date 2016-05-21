#include "GameObjects.h"
#include "Globals.h"

const char* mangoPath = "Assets/OBJ/Mango/mango.obj";
const char* chickenPath = "Assets/FBX/chicken_dance.fbx";
const char* cratePath = "Assets/OBJ/Crate/Crate1.obj";
const char* playerPath = "Assets/FBX/rig3.fbx"; // "Assets/FBX/rigtest.fbx";
const char* wizardPath = "Assets/OBJ/Wizard_Hat/wizard_hat.obj";

string skyboxDirectory = "Assets/Cubemap";
string backgroundPath = "Assets/UI/bckTmp.png";

GameObjects::GameObjects() {
}


GameObjects::~GameObjects() {
}


void GameObjects::loadGameObjects() {
	glm::vec3 pointLightPositions[] = {
		glm::vec3(2.3f, -1.6f, -3.0f),
		glm::vec3(-1.7f, 0.9f, 1.0f)
	};

	LightShader* lightShader = new LightShader(Globals::cam.getPosition(), "Shaders/shader_lighting.vert", "Shaders/shader_lighting.frag");
	lightShader->addDirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f));
	//lightShader->addDirectionalLight(glm::vec3(0.2f, 10.0f, 0.f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f));
	lightShader->addPointLight(pointLightPositions[0], glm::vec3(0.05f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.009f, 0.0032f);
	lightShader->addPointLight(pointLightPositions[1], glm::vec3(0.05f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f, 0.009f, 0.0032f);

	shaderMap[_LtShader] = lightShader;

	modelMap[_Mango] = new Model(mangoPath, shaderMap[_LtShader]);
	modelMap[_Player] = new PlayerModel(playerPath, shaderMap[_LtShader]);
	modelMap[_Crate] = new Model(cratePath, shaderMap[_LtShader]);
	modelMap[_Wizard] = new Model(wizardPath, shaderMap[_LtShader]);

	Shader *shader = new Shader("Shaders/guiItem.vert", "Shaders/guiItem.frag");
	guiMap[_Background] = new GuiItem(backgroundPath, shader, 60, 40, 0, 0);

	generateWorld(skyboxDirectory);
}

void GameObjects::generateWorld(string directory) {
	World *world = new World();
	world->createWorld(shaderMap[_LtShader], directory);
	cerr << "A" << endl;
	root = new SMatrixTransform();
	root->addNode(world);
	int width, height;
}