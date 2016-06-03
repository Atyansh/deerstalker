#pragma once

#include <vector>
#include <unordered_map>

#include "GUIText.h"
#include "Shader.h"

using namespace std;

class ReadyGUI
{
private:

	int _width, _height;

	GUIText* readyText;

public:
	ReadyGUI(int w, int h);
	~ReadyGUI();

	void draw(unordered_map<int, bool> players);
};

