#pragma once

#include "GuiItem.h"

#include <iostream>
#include <map>

#include <ft2build.h>
#include <freetype\freetype.h>

using namespace std;

struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

class GUIText : public GuiItem {

private:
	FT_Library _ft;
	FT_Face _face;

	map<GLchar, Character> Characters;
	GLuint VAO, VBO;

	Shader *_shader;

	bool initFT();
	bool initFace();

	void loadFont();
	void setupBuffers();

public:
	GUIText();
	GUIText(Shader *shader) : _shader(shader) {}
	~GUIText();

	bool init();
	void setFontSize(int size);
	void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

	void draw(DrawData &data);
	void update(UpdateData &data);
};

