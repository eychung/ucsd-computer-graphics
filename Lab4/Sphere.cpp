#include "Sphere.h"

extern bool culling;
extern bool debug;

Sphere::Sphere(int id)
{
	this->id = id;
}

void Sphere::draw(Matrix4 m)
{
	glMatrixMode(GL_MODELVIEW);
	getBoundingSphere(this->id,m);
	if (culling && sphereInFrustum(getCenterX(),getCenterY(),getCenterZ(),getRadius()))
	{
		switch (this->id)
		{
			case HEAD:
				glLoadMatrixf(m.getPointer());
				glColor3d(1.0,.8,.4);
				glutSolidTeapot(1);
				break;
			case TORSO:
				glLoadMatrixf(m.getPointer());
				glColor3d(.8,1.0,0.0);
				glutSolidCube(1);
				if (debug)
				{
					glColor3d(1.0,0.0,0.0);
					glutWireSphere(1,8,8);
				}
				break;
			case SHOULDER:
				glLoadMatrixf(m.getPointer());
				glColor3d(0.0,0.2,1.0);
				glutSolidCone(.25,1,5,5);
				glColor3d(1.0,1.0,0.0);
				glutSolidSphere(.3,5,5);
				break;
			case LEG:
				glLoadMatrixf(m.getPointer());
				glColor3d(0.0,0.2,1.0);
				glutSolidCone(.25,1,5,5);
				glColor3d(0.0,1.0,1.0);
				glutSolidSphere(.3,5,5);
				break;
			case ARM:
				glLoadMatrixf(m.getPointer());
				glColor3d(.2,0.0,.6);
				glutSolidCone(.25,1,5,5);
				glColor3d(0.0,1.0,1.0);
				glutSolidSphere(.3,5,5);
				break;
			case FOOT:
				glLoadMatrixf(m.getPointer());
				glColor3d(.2,0.0,.6);
				glutSolidCone(.25,1,5,5);
				break;
		}
	}
	else if (!culling && !sphereInFrustum(getCenterX(),getCenterY(),getCenterZ(),getRadius()))
	{
		glColor3d(0.0,0.0,0.0);
		switch (this->id)
		{
			case HEAD:
				glLoadMatrixf(m.getPointer());
				glutSolidTeapot(1);
				break;
			case TORSO:
				glLoadMatrixf(m.getPointer());
				glutSolidCube(1);
				break;
			case SHOULDER:
				glLoadMatrixf(m.getPointer());
				glutSolidCone(.25,1,5,5);
				glutSolidSphere(.3,5,5);
				break;
			case LEG:
				glLoadMatrixf(m.getPointer());
				glutSolidCone(.25,1,5,5);
				glutSolidSphere(.3,5,5);
				break;
			case ARM:
				glLoadMatrixf(m.getPointer());
				glutSolidCone(.25,1,5,5);
				glutSolidSphere(.3,5,5);
				break;
			case FOOT:
				glLoadMatrixf(m.getPointer());
				glutSolidCone(.25,1,5,5);
				break;
		}
	}
	else if (!culling && sphereInFrustum(getCenterX(),getCenterY(),getCenterZ(),getRadius()))
	{
		switch (this->id)
		{
			case HEAD:
				glLoadMatrixf(m.getPointer());
				glColor3d(1.0,.8,.4);
				glutSolidTeapot(1);
				break;
			case TORSO:
				glLoadMatrixf(m.getPointer());
				glColor3d(.8,1.0,0.0);
				glutSolidCube(1);
				break;
			case SHOULDER:
				glLoadMatrixf(m.getPointer());
				glColor3d(0.0,0.2,1.0);
				glutSolidCone(.25,1,5,5);
				glColor3d(1.0,1.0,0.0);
				glutSolidSphere(.3,5,5);
				break;
			case LEG:
				glLoadMatrixf(m.getPointer());
				glColor3d(0.0,0.2,1.0);
				glutSolidCone(.25,1,5,5);
				glColor3d(0.0,1.0,1.0);
				glutSolidSphere(.3,5,5);
				break;
			case ARM:
				glLoadMatrixf(m.getPointer());
				glColor3d(.2,0.0,.6);
				glutSolidCone(.25,1,5,5);
				glColor3d(0.0,1.0,1.0);
				glutSolidSphere(.3,5,5);
				break;
			case FOOT:
				glLoadMatrixf(m.getPointer());
				glColor3d(.2,0.0,.6);
				glutSolidCone(.25,1,5,5);
				break;
		}
	}
}