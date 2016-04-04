#include "Sphere.h"
#include "Window.h"
#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

Vector3 force;
int gravity = 0;

Sphere::Sphere(double radius, int slices, int stacks) : Drawable()
{
    this->radius = radius;
    this->slices = slices;
    this->stacks = stacks;
}

void Sphere::draw(DrawData& data)
{
    material.apply();
    
    glMatrixMode(GL_MODELVIEW);
    
    glPushMatrix();
    glMultMatrixf(toWorld.ptr());
    
    glutSolidSphere(radius, slices, stacks);
    
    glPopMatrix();
}


void Sphere::update(UpdateData& data)
{
    //
    toWorld = move() * toWorld;
    
}

Matrix4 Sphere::move(){
    Matrix4 m;
    m.identity();
    
    float wx = toWorld.ptr()[12];
    float wy = toWorld.ptr()[13];
    
    if(!gravity){
        //wall
        if(wx<-10 || wx > 10){
            force.ptr()[0] = -force.ptr()[0];
            if(wx<-10){
                toWorld.ptr()[12] = -10;
            }else{
                toWorld.ptr()[12] = 10;
            }
        }
        if(wy<-10 || wy> 10){
            force.ptr()[1] = -force.ptr()[1];
            if(wy<-10){
                toWorld.ptr()[13] = -10;
            }else{
                toWorld.ptr()[13] = 10;
            }
        }
    }else{
        force.ptr()[1] = force.ptr()[1] - 0.05;
        //wall
        if(wx<-10 || wx > 10){
            force.ptr()[0] = -force.ptr()[0];
            force.ptr()[1] = -force.ptr()[1] * 0.0005;
            if(wx<-10){
                toWorld.ptr()[12] = -10;
            }else{
                toWorld.ptr()[12] = 10;
            }
        }
        if(wy<-10 || wy> 10){
            force.ptr()[1] = -force.ptr()[1] * 0.65;
            if(wy<-10){
                toWorld.ptr()[13] = -10;
            }else{
                toWorld.ptr()[13] = 10;
            }
        }

    }

    
    m.makeTranslate(force);
    return m;
}

void Sphere::applyForce(Vector3 b){
    force = b;
}

void Sphere::toggleGravity(int g){
    gravity = g;
}


