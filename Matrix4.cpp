#include <iostream>
#include <math.h>

#include "Matrix4.h"

using namespace std;

Matrix4::Matrix4()
{
  /*for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      m[i][j] = 0;
    }
  }*/
  float ident[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      m[i][j] = ident[i][j];
    }
  }
} 

Matrix4::Matrix4(
  float m00, float m01, float m02, float m03,
  float m10, float m11, float m12, float m13,
  float m20, float m21, float m22, float m23,
  float m30, float m31, float m32, float m33 )
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

float * Matrix4::getPointer()
{
  return &m[0][0];
}

void Matrix4::identity()
{
  float ident[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
  for (int i=0; i<4; ++i)
  {
    for (int j=0; j<4; ++j)
    {
      m[i][j] = ident[i][j];
    }
  }
}

// angle in radians
Matrix4 Matrix4::rotateX(double angle)
{
	return Matrix4(1,0,0,0,0,cos(angle),sin(angle),0,0,-sin(angle),cos(angle),0,0,0,0,1);
}

// angle in radians
Matrix4 Matrix4::rotateY(double angle)
{
	return Matrix4(cos(angle),0,-sin(angle),0,0,1,0,0,sin(angle),0,cos(angle),0,0,0,0,1);
}

// angle in radians
Matrix4 Matrix4::rotateZ(double angle)
{
	return Matrix4(cos(angle),sin(angle),0,0,-sin(angle),cos(angle),0,0,0,0,1,0,0,0,0,1);
}

Matrix4 Matrix4::rotateAA(Vector4 &a, float angle)
{
	float x = a.get(0);
	float y = a.get(1);
	float z = a.get(2);
	Matrix4 r = Matrix4(1+(1-cos(angle))*(x*x-1), z*sin(angle)+(1-cos(angle))*y*x, -y*sin(angle)+(1-cos(angle))*z*x,0,
		-z*sin(angle)+(1-cos(angle))*x*y, 1+(1-cos(angle))*(y*y-1), x*sin(angle)+(1-cos(angle))*z*y,0,
		y*sin(angle)+(1-cos(angle))*x*z, -x*sin(angle)+(1-cos(angle))*y*z, 1+(1-cos(angle))*(z*z-1),0,
		0,0,0,1);
	return r;
}

Matrix4 Matrix4::scale(float x0, float y0, float z0)
{
	Matrix4 s = Matrix4(x0,0,0,0,0,y0,0,0,0,0,z0,0,0,0,0,1);
	return multiply(s);
}

Matrix4 Matrix4::translate(float x0, float y0, float z0)
{
	Matrix4 t = Matrix4(1,0,0,0,0,1,0,0,0,0,1,0,x0,y0,z0,1);
	return multiply(t);
}

Matrix4 Matrix4::multiply(Matrix4 &m)
{
	Matrix4 c = Matrix4();
	int row, row2, col;
	float x;
	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			x = 0;
			for (row2 = 0; row2 < 4; row2++)
			{
				x += this->m[row][row2] * *(m.getPointer() + (4*row2) + col);
			}
			c.m[row][col] = x;
		}
	}
	return c;
}

Vector4 Matrix4::multiply(Vector4 &v)
{
	float x = this->m[0][0]*v.x + this->m[0][1]*v.y + this->m[0][2]*v.z + this->m[0][3]*v.w;
	float y = this->m[1][0]*v.x + this->m[1][1]*v.y + this->m[1][2]*v.z + this->m[1][3]*v.w;
	float z = this->m[2][0]*v.x + this->m[2][1]*v.y + this->m[2][2]*v.z + this->m[2][3]*v.w;
	float w = this->m[3][0]*v.x + this->m[3][1]*v.y + this->m[3][2]*v.z + this->m[3][3]*v.w;
	Vector4 ans = Vector4(x,y,z,w);
	ans.dehomogenize();
	return ans;
}

Vector4 Matrix4::genMultiply(Vector4 &v)
{
	float x = this->m[0][0]*v.x + this->m[0][1]*v.y + this->m[0][2]*v.z + this->m[0][3]*v.w;
	float y = this->m[1][0]*v.x + this->m[1][1]*v.y + this->m[1][2]*v.z + this->m[1][3]*v.w;
	float z = this->m[2][0]*v.x + this->m[2][1]*v.y + this->m[2][2]*v.z + this->m[2][3]*v.w;
	float w = this->m[3][0]*v.x + this->m[3][1]*v.y + this->m[3][2]*v.z + this->m[3][3]*v.w;
	Vector4 ans = Vector4(x,y,z,w);
	return ans;
}

Matrix4 Matrix4::transpose()
{
	Matrix4 t = Matrix4();
	int i,j;
	for (i=0; i<4; i++)
	{
		for (j=0; j<4; j++)
		{
			t.m[j][i] = this->m[i][j];
		}
	}
	return t;
}

void Matrix4::setMatrix(Matrix4 &m)
{
	int i,j;
	for (i=0; i<4; i++)
	{
		for (j=0; j<4; j++)
		{
			this->m[i][j] = *(m.getPointer() + (4*i) + j);
		}
	}
}

void Matrix4::print()
{
	int row, col;
	cout << "{";
	for (row = 0; row < 4; row++)
	{
		cout << "{";
		for (col = 0; col < 4; col++)
		{
			cout << this->m[row][col] << " ";
		}
		cout << "}";
	}
	cout << "}" << endl;
}
