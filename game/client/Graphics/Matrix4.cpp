#include <math.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "Matrix4.h"
#include "Vector4.h"
#include "Vector3.h"

Matrix4::Matrix4()
{
    std::memset(m, 0, sizeof(m));
}

Matrix4::Matrix4(
                 float m00, float m01, float m02, float m03,
                 float m10, float m11, float m12, float m13,
                 float m20, float m21, float m22, float m23,
                 float m30, float m31, float m32, float m33 )
{
    this->set(m00, m01, m02, m03,
              m10, m11, m12, m13,
              m20, m21, m22, m23,
              m30, m31, m32, m33);
}

void Matrix4::set(float m00, float m01, float m02, float m03,
                  float m10, float m11, float m12, float m13,
                  float m20, float m21, float m22, float m23,
                  float m30, float m31, float m32, float m33)
{
    m[0][0] = m00;
    m[0][1] = m01;
    m[0][2] = m02;
    m[0][3] = m03;
    m[1][0] = m10;
    m[1][1] = m11;
    m[1][2] = m12;
    m[1][3] = m13;
    m[2][0] = m20;
    m[2][1] = m21;
    m[2][2] = m22;
    m[2][3] = m23;
    m[3][0] = m30;
    m[3][1] = m31;
    m[3][2] = m32;
    m[3][3] = m33;
}

float Matrix4::get(int vector,int element)
{
    return m[vector][element];
}

Matrix4& Matrix4::operator=(Matrix4 a)
{
    std::memcpy(m, a.m, sizeof(m));
    return *this;
}

float* Matrix4::ptr()
{
    return &m[0][0];
}

void Matrix4::identity()
{
    static const float ident[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    std::memcpy(m, ident, sizeof(m));
}

Matrix4 Matrix4::multiply(Matrix4 a)
{
    Matrix4 b;
    
    //The current implementation below is not very efficient:
    //It allocates an additional 8 vectors on the stack and calls their constructors
    //It also calls off to additional functions (which create even more vectors!)
    //Which results in a lot of time being wasted on memory operations
    //This is bad, really bad, ultra bad D:
    
    //Implement a more performant Matrix * Matrix multiplication
    //Hint: Loops!
    //Hint for the ambitious: SIMD!
    
//    Vector4 row1(m[0][0], m[1][0], m[2][0], m[3][0]);
//    Vector4 row2(m[0][1], m[1][1], m[2][1], m[3][1]);
//    Vector4 row3(m[0][2], m[1][2], m[2][2], m[3][2]);
//    Vector4 row4(m[0][3], m[1][3], m[2][3], m[3][3]);
//    
//    Vector4 col1(a.m[0][0], a.m[0][1], a.m[0][2], a.m[0][3]);
//    Vector4 col2(a.m[1][0], a.m[1][1], a.m[1][2], a.m[1][3]);
//    Vector4 col3(a.m[2][0], a.m[2][1], a.m[2][2], a.m[2][3]);
//    Vector4 col4(a.m[3][0], a.m[3][1], a.m[3][2], a.m[3][3]);
//
//    b.set(row1.dot(col1), row2.dot(col1), row3.dot(col1), row4.dot(col1),
//             row1.dot(col2), row2.dot(col2), row3.dot(col2), row4.dot(col2),
//             row1.dot(col3), row2.dot(col3), row3.dot(col3), row4.dot(col3),
//             row1.dot(col4), row2.dot(col4), row3.dot(col4), row4.dot(col4) );
    
    for (int c = 0; c < 4; c++){
        Vector4 row(m[0][c], m[1][c], m[2][c], m[3][c]);
        for (int r = 0; r < 4; r++){
            Vector4 col(a.m[r][0], a.m[r][1], a.m[r][2], a.m[r][3]);
            b.m[r][c] = row.dot(col);
        }
    }

    return b;
}

Matrix4 Matrix4::operator * (Matrix4 a)
{
    return multiply(a);
}

Vector4 Matrix4::multiply(Vector4 a)
{
    Vector4 b;
    
    Vector4 row1(m[0][0], m[1][0], m[2][0], m[3][0]);
    Vector4 row2(m[0][1], m[1][1], m[2][1], m[3][1]);
    Vector4 row3(m[0][2], m[1][2], m[2][2], m[3][2]);
    Vector4 row4(m[0][3], m[1][3], m[2][3], m[3][3]);
    
    b.set(row1.dot(a),row2.dot(a), row3.dot(a), row4.dot(a));
    
    return b;
}

Vector4 Matrix4::operator * (Vector4 a)
{
    return multiply(a);
}

Vector3 Matrix4::multiply(Vector3 a)
{
    Vector3 b;
    
    //Implement Matrix * Vector3 multiplication
    //Assume the 4th component is 0
    Vector3 row1(m[0][0], m[1][0], m[2][0]);
    Vector3 row2(m[0][1], m[1][1], m[2][1]);
    Vector3 row3(m[0][2], m[1][2], m[2][2]);
    
    b.set(row1.dot(a),row2.dot(a), row3.dot(a));
    
    return b;
}

Vector3 Matrix4::operator * (Vector3 a)
{
    return multiply(a);
}

Matrix4 Matrix4::makeRotateX(float angle)
{
    identity();
    
    //Configure this matrix to be a rotation about the X-Axis by 'angle' radians
    m[1][1] = cos(angle);
    m[1][2] = sin(angle);
    m[2][1] = -sin(angle);
    m[2][2] = cos(angle);
    
    return *this;
}

Matrix4 Matrix4::makeRotateY(float angle)
{
    identity();
    
	m[0][0] = cos(angle);
	m[0][2] = -sin(angle);
	m[2][0] = sin(angle);
	m[2][2] = cos(angle);
    
    return *this;
}

Matrix4 Matrix4::makeRotateZ(float angle)
{
    identity();
    
    //Configure this matrix to be a rotation about the Z-Axis by 'angle' radians
    m[0][0] = cos(angle);
    m[0][1] = sin(angle);
    m[1][0] = -sin(angle);
    m[1][1] = cos(angle);
    
    return *this;
}

Matrix4 Matrix4::makeRotateArbitrary(Vector3 a, float angle)
{
    identity();
    
    //Configure this matrix to be a rotation about the 'a' axis by 'angle' radians
    float c = cosf(angle);
    float s = sinf(angle);
    
    float x = a.ptr()[0];
    float y = a.ptr()[1];
    float z = a.ptr()[2];
    
    float one = x*x+c*(1-x*x);
    float two = x*y*(1-c)+z*s;
    float three = x*z*(1-c)-y*s;
    float four = 0;
    float five = x*y*(1-c)-z*s;
    float six = y*y+c*(1-y*y);
    float seven = y*z*(1-c)+x*s;
    float eight = 0;
    float nine = x*z*(1-c)+y*s;
    float ten = y*z*(1-c)-x*s;
    float eleven = z*z+c*(1-z*z);
    float twelve = 0;
    float thirteen = 0;
    float fourteen = 0;
    float fifteen = 0;
    float sixteen = 1;
    
    (*this).set(one, two, three, four,
                five, six, seven, eight,
                nine, ten, eleven, twelve,
                thirteen, fourteen, fifteen, sixteen);
    
    return *this;
}

Matrix4 Matrix4::makeScale(float s)
{
    return makeScale(s, s, s);
}

Matrix4 Matrix4::makeScale(float sx, float sy, float sz)
{
    identity();
    
    //Configure this matrix to be a sclaing by sx, sy, sz
    (*this).set(sx, 0, 0, 0,
                0, sy, 0, 0,
                0, 0, sz, 0,
                0, 0, 0, 1);
    return *this;
}

Matrix4 Matrix4::makeTranslate(float x, float y, float z)
{
    identity();
    
    //Configure this matrix to be a translation by vector 'a'
    (*this).set(1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                x, y, z, 1);
    
    return *this;
}

Matrix4 Matrix4::makeTranslate(Vector3 a)
{
    return makeTranslate(a[0], a[1], a[2]);
}

Matrix4 Matrix4::transpose(void)
{
    Matrix4 b;
    for(int x = 0; x < 4; ++x)
    {
        for(int y = 0; y < 4; ++y)
        {
            b.m[y][x] = m[x][y];
        }
    }
    return b;
}

//Hint: Try basing this on code by cool people on the internet
//In this class it is okay to use code from the internet
//So long as you fully understand the code and can clearly explain it if asked to
//http://stackoverflow.com/questions/2624422/efficient-4x4-matrix-inverse-affine-transform
Matrix4 Matrix4::inverse(void)
{
    Matrix4 b;
    
    //Calculate the inverse of this matrix
    //using formula from http://www.cg.info.hiroshima-cu.ac.jp/~miyazaki/knowledge/teche23.html
    
    //var for formula
    float a11 = m[0][0];
    float a12 = m[1][0];
    float a13 = m[2][0];
    float a14 = m[3][0];
    
    float a21 = m[0][1];
    float a22 = m[1][1];
    float a23 = m[2][1];
    float a24 = m[3][1];
    
    float a31 = m[0][2];
    float a32 = m[1][2];
    float a33 = m[2][2];
    float a34 = m[3][2];
    
    float a41 = m[0][3];
    float a42 = m[1][3];
    float a43 = m[2][3];
    float a44 = m[3][3];
    
    float detA = (a11*a22*a33*a44) + (a11*a23*a34*a42) + (a11*a24*a32*a43) +
                 (a12*a21*a34*a43) + (a12*a23*a31*a44) + (a12*a24*a33*a41) +
                 (a13*a21*a32*a44) + (a13*a22*a34*a41) + (a13*a24*a31*a42) +
                 (a14*a21*a33*a42) + (a14*a22*a31*a43) + (a14*a23*a32*a41) -
                 (a11*a22*a34*a43) - (a11*a23*a32*a44) - (a11*a24*a33*a42) -
                 (a12*a21*a33*a44) - (a12*a23*a34*a41) - (a12*a24*a31*a43) -
                 (a13*a21*a34*a42) - (a13*a22*a31*a44) - (a13*a24*a32*a41) -
                 (a14*a21*a32*a43) - (a14*a22*a33*a41) - (a14*a23*a31*a42);

    if(detA == 0){//not invertible, return
        return *this;
    }
    
    //inverted var for formula
    float b11 = ((a22*a33*a44)+(a23*a34*a42)+(a24*a32*a43)-(a22*a34*a43)-(a23*a32*a44)-(a24*a33*a42))/detA;
    float b12 = ((a12*a34*a43)+(a13*a32*a44)+(a14*a33*a42)-(a12*a33*a44)-(a13*a34*a42)-(a14*a32*a43))/detA;
    float b13 = ((a12*a23*a44)+(a13*a24*a42)+(a14*a22*a43)-(a12*a24*a43)-(a13*a22*a44)-(a14*a23*a42))/detA;
    float b14 = ((a12*a24*a33)+(a13*a22*a34)+(a14*a23*a32)-(a12*a23*a34)-(a13*a24*a32)-(a14*a22*a33))/detA;
    
    float b21 = ((a21*a34*a43)+(a23*a31*a44)+(a24*a33*a41)-(a21*a33*a44)-(a23*a34*a41)-(a24*a31*a43))/detA;
    float b22 = ((a11*a33*a44)+(a13*a34*a41)+(a14*a31*a43)-(a11*a34*a43)-(a13*a31*a44)-(a14*a33*a41))/detA;
    float b23 = ((a11*a24*a43)+(a13*a21*a44)+(a14*a23*a41)-(a11*a23*a44)-(a13*a24*a41)-(a14*a21*a43))/detA;
    float b24 = ((a11*a23*a34)+(a13*a24*a31)+(a14*a21*a33)-(a11*a24*a33)-(a13*a21*a34)-(a14*a23*a31))/detA;
    
    float b31 = ((a21*a32*a44)+(a22*a34*a41)+(a24*a31*a42)-(a21*a34*a42)-(a22*a31*a44)-(a24*a32*a41))/detA;
    float b32 = ((a11*a34*a42)+(a12*a31*a44)+(a14*a32*a41)-(a11*a32*a44)-(a12*a34*a41)-(a14*a31*a42))/detA;
    float b33 = ((a11*a22*a44)+(a12*a24*a41)+(a14*a21*a42)-(a11*a24*a42)-(a12*a21*a44)-(a14*a22*a41))/detA;
    float b34 = ((a11*a24*a32)+(a12*a21*a34)+(a14*a22*a31)-(a11*a22*a34)-(a12*a24*a31)-(a14*a21*a32))/detA;
    
    float b41 = ((a21*a33*a42)+(a22*a31*a43)+(a23*a32*a41)-(a21*a32*a43)-(a22*a33*a41)-(a23*a31*a42))/detA;
    float b42 = ((a11*a32*a43)+(a12*a33*a41)+(a13*a31*a42)-(a11*a33*a42)-(a12*a31*a43)-(a13*a32*a41))/detA;
    float b43 = ((a11*a23*a42)+(a12*a21*a43)+(a13*a22*a41)-(a11*a22*a43)-(a12*a23*a41)-(a13*a21*a42))/detA;
    float b44 = ((a11*a22*a33)+(a12*a23*a31)+(a13*a21*a32)-(a11*a23*a32)-(a12*a21*a33)-(a13*a22*a31))/detA;
    
    b.set(b11,b21,b31,b41,
          b12,b22,b32,b42,
          b13,b23,b33,b43,
          b14,b24,b34,b44);
    
    return b;
}

Matrix4 Matrix4::orthoNormalInverse(void)
{
    Matrix4 b;
    
    //Calculate the inverse of this matrix with the assumption that it is ortho-normal
    //This will be useful when implementing cameras!
    
    return b;
}

void Matrix4::print(std::string comment)
{
    //Width constants and variables
    static const int pointWidth = 1;
    static const int precisionWidth = 4;
    int integerWidth = 1;
    
    //Determine the necessary width to the left of the decimal point
    float* elementPtr = (float*)m;
    float maxValue = fabsf(*(elementPtr++));
    while(elementPtr++ < ((float*)m+16)) if(fabsf(*elementPtr) > maxValue) maxValue = fabsf(*elementPtr);
    while(maxValue >= 10.0) { ++integerWidth; maxValue /= 10.0; }
    
    //Sum up the widths to determine the cell width needed
    int cellWidth = integerWidth + pointWidth + precisionWidth;
    
    //Set the stream parameters for fixed number of digits after the decimal point
    //and a set number of precision digits
    std::cout << std::fixed;
    std::cout << std::setprecision(precisionWidth);
    
    //Print the comment
    std::cout << comment << std::endl;
    
    //Loop through the matrix elements, format each, and print them to screen
    float cellValue;
    for(int element = 0; element < 4; element++)
    {
        std::cout << std::setw(1) << (element == 0 ? "[" : " ");
        for(int vector = 0; vector < 4; vector++)
        {
            cellValue =  m[vector][element];
            std::cout << std::setw(cellWidth + (cellValue >= 0.0 ? 1 : 0)) << cellValue;
            std::cout << std::setw(0) << (vector < 3 ? " " : "");
        }
        std::cout << std::setw(1) << (element == 3 ? "]" : " ") << std::endl;
    }
}
