#include "PlayerGameGUI.h"

const char* fontShaderVert = "Shaders/fontShader.vert";
const char* fontShaderFrag = "Shaders/fontShader.frag";
const char* fontPath = "Assets/Font/bubblebutt.ttf";

PlayerGameGUI::PlayerGameGUI(Shader *shader, int w, int h) : _shader(shader), _width(w), _height(h) {
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(w), 0.0f, static_cast<GLfloat>(h));
	Shader* fontShader = new Shader(fontShaderVert, fontShaderFrag);
	playerName = new GUIText(fontShader, projection, fontPath);
	playerName->init();
	health = new HealthBarGUI(_shader, projection);

	// Three lives
	lives.push_back(new LivesGUI(_shader, projection));
	lives.push_back(new LivesGUI(_shader, projection));
	lives.push_back(new LivesGUI(_shader, projection));

}


PlayerGameGUI::~PlayerGameGUI() {
}

void PlayerGameGUI::draw(glm::mat4 translation) {
	// Draw everything
	playerName->renderText("P1", 200.f, 400.f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 hMat;
	glm::scale(hMat, glm::vec3(0.5f));
	glm::translate(hMat, glm::vec3(100.0f, 100.0f, 0.0f));
	
	glm::mat4 trans;
	trans = glm::scale(trans, glm::vec3(0.5f));
	trans = glm::translate(trans, glm::vec3(200.f, 200.f, 0.f));
	health->draw(trans);

	glm::mat4 lMat;
	glm::scale(lMat, glm::vec3(0.5f));
	glm::translate(lMat, glm::vec3(0.f, 200.f, 0.0f));
	for (int i = 0; i < lives.size(); i++) {
	glm::translate(lMat, glm::vec3(500.f, 0.f, 0.0f));
		lives[i]->draw(lMat);
	}

}

void PlayerGameGUI::update() {
	// Update number of lives and health
	// Update hat
}