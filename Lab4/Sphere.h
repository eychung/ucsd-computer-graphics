#ifndef _SPHERE_H_
#define _SPHERE_H_

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>

#include "Geode.h"
#include "Enum.h"
#include "../Matrix4.h"
#include "../lib/glut-3.7.6-bin/include/GL/glut.h"

class Sphere : public Geode {
	private:
		int id;
	// should consist of a draw function which uses OpenGL code to create a sphere
	// you can use glutSolidSphere to render the sphere
	public:
		Sphere(int id);
		void draw(Matrix4 m);
};

#endif