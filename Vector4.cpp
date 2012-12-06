#include <iostream>
#include <math.h>

#include "Vector4.h"

using namespace std;

Vector4::Vector4()
{
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
	this->w = 0.0;
}

Vector4::Vector4(float x0, float y0, float z0, float w0)
{
	this->x = x0;
	this->y = y0;
	this->z = z0;
	this->w = w0;
}

void Vector4::set(float x0, float y0, float z0, float w0)
{
	this->x = x0;
	this->y = y0;
	this->z = z0;
	this->w = w0;
}

float Vector4::get(int n)
{
	if (n==0) return this->x;
	else if (n==1) return this->y;
	else if (n==2) return this->z;
	else if (n==3) return this->w;
	else return -1;
}

float Vector4::operator[](int n)
{
	return get(n);
}

void Vector4::operator+(Vector4 v)
{
	add(v);
}

void Vector4::operator-(Vector4 v)
{
	subtract(v);
}

void Vector4::add(Vector4 &a)
{
	this->x+=a.x;
	this->y+=a.y;
	this->z+=a.z;
	this->w+=a.w;
}

void Vector4::add(Vector4 &a, Vector4 &b)
{
	this->x = a.x + b.x;
	this->y = a.y + b.y;
	this->z = a.z + b.z;
	this->w = a.w + b.w;
}

void Vector4::subtract(Vector4 &a)
{
	this->x-=a.x;
	this->y-=a.y;
	this->z-=a.z;
	this->w-=a.w;
}

void Vector4::subtract(Vector4 &a, Vector4 &b)
{
	this->x = a.x - b.x;
	this->y = a.y - b.y;
	this->z = a.z - b.z;
	this->w = a.w - b.w;
}

// 3D points, w = 1
// 3D vectors, w = 0
void Vector4::dehomogenize()
{
	if (this->w!=0)
	{
		this->x /= this->w;
		this->y /= this->w;
		this->z /= this->w;
		this->w /= this->w;
	}
}

Vector3 Vector4::makeVector3Point(Vector4 &v)
{
	return Vector3(v[0],v[1],v[2]);
}

float Vector4::multiply(Vector4 &v)
{
	return this->x*v.x + this->y*v.y + this->z*v.z + this->w*v.w;
}

void Vector4::print()
{
	cout << this->x << " " << this->y << " " << this->z << " " << this->w << endl;
}