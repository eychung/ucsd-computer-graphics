#include "Sphere.h"

extern bool culling;
extern bool debug;

extern int nVerts;
extern float * vertices;
extern float * normals;
extern float * texcoords;
extern int nIndices;
extern int * indices;

Sphere::Sphere(int id)
{
	this->id = id;
}

GLfloat light_diffuse[] = {1.0,0.0,0.0,1.0};
extern GLfloat mat_specular[];
extern GLfloat mat_shininess[];
extern GLfloat mat_diffuse_car[];

void drawObj()
{
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_car);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_TRIANGLES);
	for (int i=0; i<nIndices; i+=3)
	{
		glVertex3f(vertices[indices[i]*3],vertices[indices[i]*3+1],vertices[indices[i]*3+2]);
		glVertex3f(vertices[indices[i+1]*3],vertices[indices[i+1]*3+1],vertices[indices[i+1]*3+2]);
		glVertex3f(vertices[indices[i+2]*3],vertices[indices[i+2]*3+1],vertices[indices[i+2]*3+2]);

		if (normals != NULL)
		{
			glNormal3f(normals[indices[i]*3],normals[indices[i]*3+1],normals[indices[i]*3+2]);
			glNormal3f(normals[indices[i+1]*3],normals[indices[i+1]*3+1],normals[indices[i+1]*3+2]);
			glNormal3f(normals[indices[i+2]*3],normals[indices[i+2]*3+1],normals[indices[i+2]*3+2]);
		}
		if (texcoords != NULL)
		{
			glTexCoord3f(texcoords[indices[i]*3],texcoords[indices[i]*3+1],texcoords[indices[i]*3+2]);
			glTexCoord3f(texcoords[indices[i+1]*3],texcoords[indices[i+1]*3+1],texcoords[indices[i+1]*3+2]);
			glTexCoord3f(texcoords[indices[i+2]*3],texcoords[indices[i+2]*3+1],texcoords[indices[i+2]*3+2]);
		}
	}
	glEnd();
}


void Sphere::draw(Matrix4 m)
{
	glMatrixMode(GL_MODELVIEW);
	getBoundingSphere(this->id,m);
	if (culling && sphereInFrustum(getCenterX(),getCenterY(),getCenterZ(),getRadius()))
	{
		switch (this->id)
		{
			case CAR:
				glLoadMatrixf(m.getPointer());
				drawObj();
				break;
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