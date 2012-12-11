#include "Sphere.h"

extern bool culling;
extern bool debug;

extern int nVerts;
extern float * vertices;
extern float * normals;
extern float * texcoords;
extern int nIndices;
extern int * indices;

using namespace std;

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


extern bool toggle_shader;
extern Matrix4 m_world;

void Sphere::draw(Matrix4 m)
{
	glMatrixMode(GL_MODELVIEW);
	
	if (intersectCharacter(m))
		glColor3d(1.0,0.0,0.0);
	else
		glColor3d(0.8,1.0,0.0);

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
	}

	if (debug)
	{
		switch (this->id)
		{
			case HEAD:
				glColor3d(0.0,0.0,1.0);
				glutWireSphere(2,8,8);
				break;
			case TORSO:
				glColor3d(0.0,0.0,1.0);
				glutWireSphere(1,8,8);
				break;
		}
	}
}