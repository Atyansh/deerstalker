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
    
    DrawData(void);
    ~DrawData(void);
    
};
