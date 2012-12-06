#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include "Vector4.h"

class Matrix4
{
  protected:
    float m[4][4];   // matrix elements
    
  public:
    Matrix4();        // constructor
    Matrix4(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);
    float * getPointer();  // return pointer to matrix elements
    void identity();  // create identity matrix
	Matrix4 rotateX(double);
    Matrix4 rotateY(double); // rotation about y axis
	Matrix4 rotateZ(double);
	Matrix4 rotateAA(Vector4 &a, float angle);
	Matrix4 scale(float x0, float y0, float z0);
	Matrix4 translate(float x0, float y0, float z0);
	
	Matrix4 multiply(Matrix4 &m);
	Vector4 multiply(Vector4 &v);
	Vector4 genMultiply(Vector4 &v);

	void setMatrix(Matrix4 &m);
	Matrix4 transpose();

	void print();
};

#endif