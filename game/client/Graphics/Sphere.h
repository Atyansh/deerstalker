#ifndef CSE167_Sphere_h
#define CSE167_Sphere_h

#include <iostream>
#include "Drawable.h"
#include "Matrix4.h"
#include "Vector3.h"

class Sphere : public Drawable
{
    
public:
    
    double radius;
    int slices, stacks;
    
    Sphere(double, int, int);
    
    virtual void draw(DrawData&);
    virtual void update(UpdateData&);
    
    //
    void applyForce(Vector3 b);
    void toggleGravity(int g);
    Matrix4 move();
    
};

#endif
