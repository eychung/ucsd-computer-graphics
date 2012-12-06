#ifndef _VECTOR3_H_
#define _VECTOR3_H_

//#include "Vector4.h"

class Vector4;

class Vector3
{
	public:
		float x, y, z;
		
		Vector3();
		Vector3(float x0, float y0, float z0);

		void set(float x0, float y0, float z0);
		float get(int n);
		float operator[](int n);
		
		Vector3 add(Vector3 &a);
		Vector3 add(Vector3 &a, Vector3 &b);
		Vector3 operator+(Vector3 v);
		
		Vector3 subtract(Vector3 &a);
		Vector3 subtract(Vector3 &a, Vector3 &b);
		Vector3 operator-(Vector3 v);

		Vector3 negate();
		Vector3 scale(float s);
		Vector3 scale(float s, Vector3 &a);
		float dot(Vector3 &a);
		Vector3 cross(Vector3 &a, Vector3 &b);
		float magnitude();
		Vector3 normalize();

		void print();
};

#endif