#pragma once

#include <vector>
#include <string>

#include "HealthBarGUI.h"
#include "GUIText.h"
#include "LivesGUI.h"
#include "Shader.h"


class PlayerGameGUI {
private:
	
	Shader *_shader;
	int _width, _height;
	string _name;

	float _currHealth;
	int _livesLeft;

	// Will have hat image
	GUIText *playerName;
	HealthBarGUI *health;
	vector <LivesGUI *> lives;

public:
	PlayerGameGUI(Shader *shader, int w, int h, string name);
	~PlayerGameGUI();

	// Beacause we want to draw the stuff in othro projection
	// gonna be different than all the draws with DrawData.
	// Will refactor when time comes.
	void draw(glm::mat4 translation);
	void update(int updatedLives, int updatedHealth, int updatedHat);
};

