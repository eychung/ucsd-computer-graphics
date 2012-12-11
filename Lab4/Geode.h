// abstract subclasses Group
#ifndef _GEODE_H_
#define _GEODE_H_

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>

#include "Node.h"
#include "../Vector3.h"
#include "Enum.h"
#include "../Matrix4.h"

class Geode : public Node {
	protected:
		float centerX;
		float centerY;
		float centerZ;
		float radius;
	public:
		void setPos(float x, float y, float z)
		{
			centerX = x;
			centerY = y;
			centerZ = z;
		}

		float getCenterX() { return centerX; }
		float getCenterY() { return centerY; }
		float getCenterZ() { return centerZ; }
		float getRadius() { return radius; }

		bool intersectCharacter(Matrix4 m)
		{
			float c_x = *(m.getPointer() + 12);
			float c_y = *(m.getPointer() + 13);
			float c_z = *(m.getPointer() + 14);
			if (11 > c_x && -11 < c_x &&
				8 > c_y && -8 < c_y &&
				11 > c_z && -11 < c_z) 
				return true;
			return false;
		}
		virtual void draw(Matrix4 m) = 0;
};

#endif