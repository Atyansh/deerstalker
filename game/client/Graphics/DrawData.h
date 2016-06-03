#pragma once

#include <glm\glm.hpp>
#include <iostream>

class DrawData
{
    
public:
    glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 matrix;
	float animTime;
	int playerId;
	int width;
	int height;
    
    DrawData(void);
    ~DrawData(void);
    
};
