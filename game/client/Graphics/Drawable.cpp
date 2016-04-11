#include "Drawable.h"
#include "Window.h"

Drawable::Drawable()
{
	this->toWorld = glm::mat4(1.0f);
}

Drawable::~Drawable()
{
    //
}

void Drawable::draw(DrawData& data)
{
    std::cout << "Warning: draw() is not implemented" << std::endl;
}

void Drawable::update(UpdateData& data)
{
    std::cout << "Warning: update() is not implemented" << std::endl;
}
