#include "ReadyGUI.h"

const char* ftShaderVert = "Shaders/fontShader.vert";
const char* ftShaderFrag = "Shaders/fontShader.frag";
const char* ftPath = "Assets/Font/bubblebutt.ttf";

ReadyGUI::ReadyGUI(int w, int h) : _width(w), _height(h) {
	
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(w), 0.0f, static_cast<GLfloat>(h));
	Shader* fontShader = new Shader(ftShaderVert, ftShaderFrag);
	readyText = new GUIText(fontShader, projection, ftPath, 100);
	readyText->init();
}


ReadyGUI::~ReadyGUI()
{
}

void ReadyGUI::draw(unordered_map<int, bool> players) {
	unordered_map<int, bool>::iterator beg = players.begin();
	float offset = 125.0f;
	while (beg != players.end()) {
		if (beg->second) {
			std::cerr << "FOUND TRUE" << std::endl;
			cerr << beg->first << endl;
			readyText->renderText("READY!", offset + (float(beg->first-1) * 460.0f), 900.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
		}
		beg++;
	}
}
