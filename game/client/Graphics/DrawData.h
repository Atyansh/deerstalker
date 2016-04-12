#ifndef CSE167_RenderData_h
#define CSE167_RenderData_h

#include <glm\glm.hpp>

#include <iostream>

class DrawData
{
    
public:
    
    //Payload
    //Place any objects here that you want to pass to a draw() call

	glm::mat4 projection;
	glm::mat4 view;
    
    DrawData(void);
    ~DrawData(void);
    
};

#endif
