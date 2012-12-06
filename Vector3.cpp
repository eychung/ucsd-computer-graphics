#include <iostream>
#include <math.h>

#include "Vector3.h"

using namespace std;

Vector3::Vector3()
{
}

Vector3::Vector3(float x0, float y0, float z0)
{
	this->x = x0;
	this->y = y0;
	this->z = z0;
}

void Vector3::set(float x0, float y0, float z0)
{
	this->x = x0;
	this->y = y0;
	this->z = z0;
}

float Vector3::get(int n)
{
	if (n==0) return this->x;
	else if (n==1) return this->y;
	else if (n==2) return this->z;
	else return -1;
}

float Vector3::operator[](int n)
{
	return get(n);
}

Vector3 Vector3::operator+(Vector3 v)
{
	return add(v);
}

Vector3 Vector3::operator-(Vector3 v)
{
	return subtract(v);
}

Vector3 Vector3::add(Vector3 &a)
{
	return Vector3(this->x+a.x, this->y+a.y, this->z+a.z);
}

Vector3 Vector3::add(Vector3 &a, Vector3 &b)
{
	return Vector3(a.x+b.x, a.y+b.y, a.z+b.z);
}

Vector3 Vector3::subtract(Vector3 &a)
{
	return Vector3(this->x-a.x, this->y-a.y, this->z-a.z);
}

Vector3 Vector3::subtract(Vector3 &a, Vector3 &b)
{
	return Vector3(a.x-b.x, a.y-b.y, a.z-b.z);
}

Vector3 Vector3::negate()
{
	return Vector3(this->x*-1, this->y*-1, this->z*-1);
}

Vector3 Vector3::scale(float s)
{
	return Vector3(this->x*s, this->y*s, this->z*s);
}

Vector3 Vector3::scale(float s, Vector3 &a)
{
	return Vector3(s*a.x, s*a.y, s*a.z);
}

float Vector3::dot(Vector3 &a)
{
	return this->x * a.x + this->y * a.y + this->z * a.z;
}

Vector3 Vector3::cross(Vector3 &a, Vector3 &b)
{
	return Vector3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

float Vector3::magnitude()
{
	return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
}

Vector3 Vector3::normalize()
{
	return scale(1.0/magnitude());
}

/*Vector4 Vector3::makeVector4Point(Vector3 &v)
{
	return Vector4(v[0],v[1],v[2],1);
}*/

void Vector3::print()
{
	cout << this->x << " " << this->y << " " << this->z << endl;
}