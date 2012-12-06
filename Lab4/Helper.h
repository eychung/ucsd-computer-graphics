#ifndef _HELPER_H_
#define _HELPER_H_

#include "../Vector3.h"
#include "../Vector4.h"
#include "../Matrix4.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "..\lib\glut-3.7.6-bin\include\GL\glut.h"

#define TERRAIN_ERROR_MEMORY_PROBLEM	-3
#define	TERRAIN_ERROR_NOT_SAVED			-2
#define TERRAIN_ERROR_NOT_INITIALISED	-1
#define TERRAIN_OK						 0

using namespace std;

class Helper
{
	public:
		Helper();
		Vector4 multiply(Vector4 &v, Matrix4 &m); // doesn't work
		float multiply(Vector4 &a, Vector4 &b); // doesn't work
		Vector3 getBezierPoint(float u, float v, Matrix4 &m_x, Matrix4 &m_y, Matrix4 &m_z);
		Vector3 getNormal(float u, float v, Matrix4 &m_x, Matrix4 &m_y, Matrix4 &m_z);
};

class TerrainHelper
{
	public:
		int terrainLoad(int width, int height, int normals);
		int terrainCreateDL(float xOffset, float yOffset, float zOffset);
		void terrainDestroy();
		int terrainScale(float min,float max);
		float terrainGetHeight(int x, int z);
		~TerrainHelper();
};

#endif