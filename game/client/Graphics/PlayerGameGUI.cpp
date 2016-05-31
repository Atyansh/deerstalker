#include "PlayerGameGUI.h"

const char* fontShaderVert = "Shaders/fontShader.vert";
const char* fontShaderFrag = "Shaders/fontShader.frag";
const char* fontPath = "Assets/Font/bubblebutt.ttf";

PlayerGameGUI::PlayerGameGUI(Shader *shader, int w, int h, string name) : _shader(shader), _width(w), _height(h), _name(name) {
	
	_livesLeft = 3;
	_currHealth = 1.0;

	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(w), 0.0f, static_cast<GLfloat>(h));
	Shader* fontShader = new Shader(fontShaderVert, fontShaderFrag);
	playerName = new GUIText(fontShader, projection, fontPath, 100);
	playerName->init();

	//playerName->setFontSize(150);
	health = new HealthBarGUI(_shader, projection);

	// Three lives
	for (int i = 0; i < _livesLeft; i++) {
		lives.push_back(new LivesGUI(_shader, projection));
	}

}


PlayerGameGUI::~PlayerGameGUI() {
}

void PlayerGameGUI::draw(glm::mat4 translation, glm::vec3 color) {
	// Draw everything

	glm::vec4 loc(30.f, 150.f, 0.0f, 1.0f);
	loc = translation * loc;
	playerName->renderText(_name, loc.x, loc.y, 1.0f, color);
	glm::mat4 trans;
	trans = glm::scale(trans, glm::vec3(0.5f, 0.4f, 1.0f));
	trans = glm::translate(trans, glm::vec3(0.0f, 220.f, 0.f));
	trans = translation * trans;
	health->draw(trans);

	glm::mat4 lMat;
	lMat = glm::scale(lMat, glm::vec3(0.3f));
	lMat = glm::translate(lMat, glm::vec3(80.0f, 150.f, 0.0f));
	lMat = translation * lMat;
	for (int i = 0; i < _livesLeft; i++) {
		if (i > 0) { lMat = glm::translate(lMat, glm::vec3(200.f, 0.f, 0.0f)); } 
		lives[i]->draw(lMat);
	}

}

void PlayerGameGUI::update(int updatedLives, int updatedHealth, int updatedHat) {
	_livesLeft = updatedLives;
	_currHealth = float(updatedHealth)/100.0f;
	health->update(_currHealth);
	// Update hats
}