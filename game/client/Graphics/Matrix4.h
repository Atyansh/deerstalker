#ifndef CSE167_Matrix4_h
#define CSE167_Matrix4_h

#include <string>

class Vector3;
class Vector4;

class Matrix4
{
    
protected:
    
    //These are in column-major
    //The first index is for the column, and the second is for the row
    //Ex: m[2][1] is the 3rd column (index 2) and second element in that column (index 1)
    //This is the matrix format used by OpenGL internally.
    float m[4][4];
    
public:
    
    Matrix4();
    Matrix4(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);
    
    void set(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);
    float get(int,int);
    
    Matrix4& operator = (Matrix4);
    
    float* ptr();
    
    void identity();
    
    Matrix4 multiply( Matrix4);
    Matrix4 operator * (Matrix4);
    Vector4 multiply( Vector4);
    Vector4 operator * (Vector4);
    Vector3 multiply( Vector3);
    Vector3 operator * (Vector3);
    
    Matrix4 makeRotateX(float);
    Matrix4 makeRotateY(float);
    Matrix4 makeRotateZ(float);
    Matrix4 makeRotateArbitrary(Vector3, float);
    
    Matrix4 makeScale(float,float,float);
    Matrix4 makeScale(float);
    
    Matrix4 makeTranslate(float,float,float);
    Matrix4 makeTranslate(Vector3);
    
    Matrix4 transpose(void);
    Matrix4 inverse(void);
    Matrix4 orthoNormalInverse(void);
    
    void print(std::string);
    
};

#endif
