#pragma once

#include "Shader.h"

class FontShader : public Shader {
private:
	int _width;
	int _height;
	//void setupProjection();
public:
	FontShader();
	FontShader(int width, int height, const char *vs, const char *fg, bool isFile = true);
	~FontShader();

	//void updateProjection(int width, int height);

	void bind();
	void unbind();
};

