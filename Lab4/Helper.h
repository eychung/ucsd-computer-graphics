#ifndef _HELPER_H_
#define _HELPER_H_

#include "../Vector3.h"
#include "../Vector4.h"
#include "../Matrix4.h"

class Helper
{
	public:
		Helper();
		Vector4 multiply(Vector4 &v, Matrix4 &m); // doesn't work
		float multiply(Vector4 &a, Vector4 &b); // doesn't work
		Vector3 getBezierPoint(float u, float v, Matrix4 &m_x, Matrix4 &m_y, Matrix4 &m_z);
		Vector3 getNormal(float u, float v, Matrix4 &m_x, Matrix4 &m_y, Matrix4 &m_z);
};

#endif