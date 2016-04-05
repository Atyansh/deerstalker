#include "Vector3.h"
#include <math.h>
#include <iostream>
#include <cstring>
#include "Vector4.h"

Vector3::Vector3()
{
    std::memset(m, 0, sizeof(m));
}

Vector3::Vector3(float m0, float m1, float m2)
{
    m[0] = m0;
    m[1] = m1;
    m[2] = m2;
}

Vector3::~Vector3()
{
    // NA
}

float* Vector3::ptr()
{
    return &m[0];
}

void Vector3::set(float x, float y, float z)
{
    m[0] = x;
    m[1] = y;
    m[2] = z;
}

void Vector3::set(int index, float value)
{
    m[index] = value;
}

float& Vector3::operator [] (int loc)
{
    return m[loc];
}

Vector3 Vector3::add(Vector3 a)
{
    Vector3 b;
    
    //add mx + ax, my + ay, mz + az
    b[0] = m[0] + a[0];
    b[1] = m[1] + a[1];
    b[2] = m[2] + a[2];
    
    return b;
}

Vector3 Vector3::operator + (Vector3 a)
{
    return add(a);
}

Vector3 Vector3::subtract(Vector3 a)
{
    Vector3 b;
    
    //sub mx - ax, my - ay, mz - az
    b[0] = m[0] - a[0];
    b[1] = m[1] - a[1];
    b[2] = m[2] - a[2];
    
    return b;
}

Vector3 Vector3::operator - (Vector3 a)
{
    return subtract(a);
}

Vector3 Vector3::negate(void)
{
    Vector3 b;
    
    // -m
    b[0] = -m[0];
    b[1] = -m[1];
    b[2] = -m[2];
    
    return b;
}

Vector3 Vector3::scale(float s)
{
    Vector3 b;
    
    // sm
    b[0] = s * m[0];
    b[1] = s * m[1];
    b[2] = s * m[2];
    
    return b;
}

Vector3 Vector3::multiply(float a)
{
    Vector3 b;
    
    //for overloading, use scale
    b = scale(a);
    
    return b;
}

Vector3 Vector3::operator * (float a)
{
    return multiply(a);
}

Vector3 Vector3::multiply(Vector3 a)
{
    Vector3 b;
    
    //Coomponent-wise multiplication
    // mx * ax, my * ay, mz * az
    b[0] = m[0] * a[0];
    b[1] = m[1] * a[1];
    b[2] = m[2] * a[2];
    
    return b;
}

Vector3 Vector3::operator * (Vector3 a)
{
    return multiply(a);
}


float Vector3::dot(Vector3 a)
{
    //mxax + myay + mzaz
    
    return (m[0]*a[0]) + (m[1]*a[1]) + (m[2]*a[2]);
}

Vector3 Vector3::cross(Vector3 a)
{
    Vector3 b;
    
    //myaz - mzay, mzax - mxaz, mxay - myax
    b[0] = (m[1]*a[2]) - (m[2]*a[1]);
    b[1] = (m[2]*a[0]) - (m[0]*a[2]);
    b[2] = (m[0]*a[1]) - (m[1]*a[0]);
    
    return b;
}

float Vector3::angle(Vector3 a)
{
    //cos−1 (( a ⋅ b ) / ( |a| |b| ))
    
    return acosf(dot(a)/(magnitude() * a.magnitude()));
}

float Vector3::magnitude(void)
{
    // (vx^2+vy^2+vz^2)^.5
    float x = m[0]*m[0];
    float y = m[1]*m[1];
    float z = m[2]*m[2];
    
    return sqrtf(x+y+z);
}

Vector3 Vector3::normalize(void)
{
    Vector3 b;
    
    // v / |v|
    float mm = magnitude();
    if (mm==0){ //error
        b.set(0.0, 0.0, 0.0);
        return b;
    }
    
    b[0] = m[0]/mm;
    b[1] = m[1]/mm;
    b[2] = m[2]/mm;
    
    return b;
}

Vector4 Vector3::toVector4(float w)
{
    Vector4 b(m[0], m[1], m[2], w);
    return b;
}

void Vector3::print(std::string comment)
{
    std::cout << comment << std::endl;
    std::cout << "<x:" << m[0] <<  ", y:" << m[1] << ", z:" << m[2] << ">" << std::endl;
}
