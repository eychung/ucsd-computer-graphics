#ifndef _PROJECT4_H_
#define _PROJECT4_H_

#pragma comment(lib, "jpeg.lib")

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "shader.h"
#include "lib\glut-3.7.6-bin\include\GL\glut.h"

#include "Lab4\Enum.h"
#include "Lab4\Constants.h"
#include "Matrix4.h"
#include "Lab4\Helper.h"
#include "Camera.h"
#include "objreader.h"

#include "Lab4\Node.h"
#include "Lab4\Group.h"
#include "Lab4\Geode.h"
#include "Lab4\MatrixTransform.h"
#include "Lab4\Sphere.h"

#define SKYFRONT 0						// Give Front ID = 0
#define SKYBACK  1						// Give Back  ID = 1
#define SKYLEFT  2						// Give Left  ID = 2
#define SKYRIGHT 3						// Give Right ID = 3
#define SKYUP    4						// Give Up    ID = 4
#define SKYDOWN  5						// Give Down  ID = 5

class Cube
{
	protected:
		Matrix4 matrix;                 // model matrix

	public:
		Cube();   // Constructor

		Matrix4 & getMatrix();
		void drawDebugControlPointsRight();
		void drawDebugControlPointsLeft();
		void drawFlag();
		void drawPlane();
		void drawFrustum();
		void drawObj();
};


class window	  // output window related routines
{
  public:
    static int width, height; 	            // window size

    static void idleCallback(void);
    static void reshapeCallback(int, int);
    static void displayCallback(void);
};

#endif

