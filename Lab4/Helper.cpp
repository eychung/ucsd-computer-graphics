#include "Helper.h"

Helper::Helper()
{
}

Vector4 Helper::multiply(Vector4 &v, Matrix4 &m)
{
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
	float w = 0.0;
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
		{
			if (i==0)
				x += v.get(i)* *(m.getPointer()+i*(j));
			else if (i==1)
				y += v.get(i)* *(m.getPointer()+i*(j));
			else if (i==2)
				z += v.get(i)* *(m.getPointer()+i*(j));
			else
				w += v.get(i)* *(m.getPointer()+i*(j));
		}
	}
	return Vector4(x,y,z,w);
}

float Helper::multiply(Vector4 &a, Vector4 &b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

Vector3 Helper::getBezierPoint(float u, float v, Matrix4 &m_x, Matrix4 &m_y, Matrix4 &m_z)
{
	Vector4 v4_v = Vector4(v*v*v,v*v,v,1.0);
	Vector4 v4_u = Vector4(u*u*u,u*u,u,1.0);

	float x = m_x.genMultiply(v4_u).multiply(v4_v);
	float y = m_y.genMultiply(v4_u).multiply(v4_v);
	float z = m_z.genMultiply(v4_u).multiply(v4_v);

	return Vector3(x,y,z);
}

Vector3 Helper::getNormal(float u, float v, Matrix4 &m_x, Matrix4 &m_y, Matrix4 &m_z)
{
	Vector4 v4_v = Vector4(v*v*v,v*v,v,1.0);
	Vector4 v4_u = Vector4(3*u*u,2*u,1.0,0.0);

	float x = m_x.genMultiply(v4_u).multiply(v4_v);
	float y = m_y.genMultiply(v4_u).multiply(v4_v);
	float z = m_z.genMultiply(v4_u).multiply(v4_v);

	Vector3 v3_u = Vector3(x,y,z);


	Vector4 v4_v2 = Vector4(3*v*v,2*v,1.0,0.0);
	Vector4 v4_u2 = Vector4(u*u*u,u*u,u,1.0);
	float x2 = m_x.genMultiply(v4_u2).multiply(v4_v2);
	float y2 = m_y.genMultiply(v4_u2).multiply(v4_v2);
	float z2 = m_z.genMultiply(v4_u2).multiply(v4_v2);
	Vector3 v3_v = Vector3(x2,y2,z2);

	return v3_u.cross(v3_v,v3_u);
}

