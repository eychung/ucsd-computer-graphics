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
		int id;
		int size;
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
			switch (size)
			{
				case SIZE1:
					if (4 > c_x && -4 < c_x && 4 > c_y && -4 < c_y && -2 > c_z && -4 < c_z) 
					{
						//cout << c_x << " and " << c_y << " and " << c_z << endl;
						return true;
					}
					break;
				case SIZE2:
					if (6 > c_x && -6 < c_x && 6 > c_y && -6 < c_y && 2 > c_z && -8 < c_z)
					{
						//cout << c_x << " and " << c_y << " and " << c_z << endl;
						return true;
					}
					break;
				case SIZE3:
					if (8 > c_x && -8 < c_x && 8 > c_y && -8 < c_y && 6 > c_z && -12 < c_z)
					{
						//cout << c_x << " and " << c_y << " and " << c_z << endl;
						return true;
					}
					break;
				case SIZE4:
					if (10 > c_x && -10 < c_x && 10 > c_y && -10 < c_y && 12 > c_z && -16 < c_z) 
					{
						//cout << c_x << " and " << c_y << " and " << c_z << endl;
						return true;
					}
					break;
			}
			return false;
		}
		virtual void draw(Matrix4 m) = 0;
};

#endif