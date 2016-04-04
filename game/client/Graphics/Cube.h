#ifndef CSE167_Cube_h
#define CSE167_Cube_h

#include "Drawable.h"

class Cube : public Drawable
{
    
public:
    
    float size;
    
    Cube(float);
    virtual ~Cube(void);
    
    virtual void draw(DrawData&);
    virtual void update(UpdateData&);
    
    void spin(float);
    
};

#endif

