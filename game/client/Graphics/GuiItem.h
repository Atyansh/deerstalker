#pragma once

#include <string>
#include <SOIL/SOIL.h>
#include "Drawable.h"
#include "Shader.h"

using namespace std;
class GuiItem : public Drawable{
	protected:
		GLuint textureID;
		Shader *shader;
		string filename;

		GLuint VAO;
		GLuint VBO;
		GLuint EBO;

		int xPos;
		int yPos;

		void init(int width, int height);

	public:
		GuiItem() {}
		GuiItem(string filename, Shader *shader, int w, int h, int x, int y);
		~GuiItem();

		void setPosition(int x, int y);
		void translate(int x, int y);
		
		int getXPos();
		int getYPos();

		virtual void draw(DrawData &data);
		virtual void update(UpdateData &data);
};
