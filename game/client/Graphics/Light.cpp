#include "Light.h"
#include "Window.h"
#include <iostream>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif


Light::Light()
{
    ambientColor = Color::ambientDefault();
    diffuseColor = Color::diffuseDefault();
    specularColor = Color::specularDefault();
    
    constantAttenuation = 1.0;
    linearAttenuation = 0.0;
    quadraticAttenuation = 0.05;
}

Light::~Light()
{
    //Delete any dynamically allocated memory/objects here
}

void Light::bind(int id)
{
    if(id < 0 || id > 7) {
        std::cout << "ERROR: The light bind ID " << id << " is not valid!" << std::endl;
        return;
    }
    
    //Set the bindID
    bindID = id;
    
    //Configure the light at the bindID
    glEnable(GL_LIGHT0 + bindID);
    
    //Configure the color of the light
    glLightfv(GL_LIGHT0 + bindID, GL_AMBIENT, ambientColor.ptr());
    glLightfv(GL_LIGHT0 + bindID, GL_DIFFUSE, diffuseColor.ptr());
    glLightfv(GL_LIGHT0 + bindID, GL_SPECULAR, specularColor.ptr());
    
    //Configure the attenuation properties of the light
    //Constant Attenuation
    //Linear Attenuation
    glLightf(GL_LIGHT0 + bindID, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);
    
    //Position the light
    glLightfv(GL_LIGHT0 + bindID, GL_POSITION, position.ptr());
    
    //Setup spotlight direction, angle, and exponent here
}

void Light::unbind(void)
{
    glDisable(GL_LIGHT0 + bindID);
    bindID = -1;
}
