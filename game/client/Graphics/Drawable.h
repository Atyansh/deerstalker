#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <float.h>
#include <math.h>
#include <vector>
#include <iostream>
#include "DrawData.h"
#include "UpdateData.h"


class Drawable
{
    
public:
    
    glm::mat4 toWorld;
    
    Drawable(void);
    ~Drawable(void);
    
    virtual void draw(DrawData&);
    virtual void update(UpdateData&);
    
};