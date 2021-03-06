#include "Drawable.h"

Drawable::Drawable()
{
	this->toWorld = glm::mat4(1.0f);

	toWorld = glm::translate(toWorld, glm::vec3(0.0f, -5.0f, -20.0f));
	toWorld = glm::scale(toWorld, glm::vec3(0.8f));
}

Drawable::~Drawable()
{
    
}

void Drawable::draw(DrawData& data)
{
    std::cout << "Warning: draw() is not implemented" << std::endl;
}

void Drawable::update(UpdateData& data)
{
    std::cout << "Warning: update() is not implemented" << std::endl;
}
