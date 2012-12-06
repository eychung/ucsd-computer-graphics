#ifndef _VECTOR4_H_
#define _VECTOR4_H_

#include "Vector3.h"

class Vector4
{
	public:
		float x, y, z, w;
		
		Vector4();
		Vector4(float x0, float y0, float z0, float w0);

		void set(float x0, float y0, float z0, float w0);
		float get(int n);
		float operator[](int n);
		
		void add(Vector4 &a);
		void add(Vector4 &a, Vector4 &b);
		void operator+(Vector4 v);
		
		void subtract(Vector4 &a);
		void subtract(Vector4 &a, Vector4 &b);
		void operator-(Vector4 v);

		void dehomogenize();

		float multiply(Vector4 &v);
		Vector3 makeVector3Point(Vector4 &v);

		void print();
};

#endif