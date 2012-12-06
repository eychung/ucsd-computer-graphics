// abstract subclasses Group
#ifndef _GEODE_H_
#define _GEODE_H_

#include "Node.h"
#include "../Vector3.h"
#include "Enum.h"
#include "../Matrix4.h"

extern float frustum[6][4];
extern Vector4 frustum_points[8];

class Geode : public Node {
	protected:
		float centerX;
		float centerY;
		float centerZ;
		float radius;
	public:
		float getCenterX() { return centerX; }
		float getCenterY() { return centerX; }
		float getCenterZ() { return centerX; }
		float getRadius() { return radius; }
		void getBoundingSphere(int id, Matrix4 m)
		{
			if (id != FOOT)
			{
				radius = 1;
			}
			else {
				radius = .25;
			}
			centerX = *(m.getPointer() + 12);
			centerY = *(m.getPointer() + 13);
			centerZ = *(m.getPointer() + 14);
		}
		bool sphereInFrustum(float x, float y, float z, float radius)
		{
			int p;
			for (p=0; p<6; p++)
			if (frustum[p][0]*x + frustum[p][1]*y + frustum[p][2]*z + frustum[p][3] <= -radius)
				return false;
			return true;
		}
		virtual void draw(Matrix4 m) = 0;
};

#endif