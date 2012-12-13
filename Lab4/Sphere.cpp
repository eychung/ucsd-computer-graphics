#include "Sphere.h"

extern bool culling;
extern bool debug;

extern int nVerts;
extern float * vertices;
extern float * normals;
extern float * texcoords;
extern int nIndices;
extern int * indices;

extern bool toggle_shader;
extern void initParticles(float xsrc, float ysrc, float zsrc);
extern void updateParticles();
extern void drawParticles();

GLfloat light_diffuse[] = {1.0,0.0,0.0,1.0};
extern GLfloat mat_specular[];
extern GLfloat mat_shininess[];
extern GLfloat mat_diffuse_car[];

static bool do_once = true;

using namespace std;

Sphere::Sphere(int id)
{
	this->id = id;
}

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

void drawBoundingBox()
{
	glBegin(GL_LINE_STRIP);
	// Draw front face:
	glNormal3f(0.0, 0.0, 1.0);   
	glVertex3f(-1.5,  0.8,  1.0);
	glVertex3f( 1.7,  0.8,  1.0);
	glVertex3f( 1.7, -0.8,  1.0);
	glVertex3f(-1.5, -0.8,  1.0);

	// Draw left side:
	glNormal3f(-1.0, 0.0, 0.0);
	glVertex3f(-1.5,  0.8,  1.0);
	glVertex3f(-1.5,  0.8, -1.0);
	glVertex3f(-1.5, -0.8, -1.0);
	glVertex3f(-1.5, -0.8,  1.0);

	// Draw right side:
	glNormal3f(1.0, 0.0, 0.0);
	glVertex3f( 1.7,  0.8,  1.0);
	glVertex3f( 1.7,  0.8, -1.0);
	glVertex3f( 1.7, -0.8, -1.0);
	glVertex3f( 1.7, -0.8,  1.0);

	// Draw back face:
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(-1.5,  0.8, -1.0);
	glVertex3f( 1.7,  0.8, -1.0);
	glVertex3f( 1.7, -0.8, -1.0);
	glVertex3f(-1.5, -0.8, -1.0);

	// Draw top side:
	glNormal3f(0.0, 1.0, 0.0);
	glVertex3f(-1.5,  0.8,  1.0);
	glVertex3f( 1.7,  0.8,  1.0);
	glVertex3f( 1.7,  0.8, -1.0);
	glVertex3f(-1.5,  0.8, -1.0);

	// Draw bottom side:
	glNormal3f(0.0, -1.0, 0.0);
	glVertex3f(-1.5, -0.8, -1.0);
	glVertex3f( 1.7, -0.8, -1.0);
	glVertex3f( 1.7, -0.8,  1.0);
	glVertex3f(-1.5, -0.8,  1.0);
	glEnd();
}

void Sphere::draw(Matrix4 m)
{
	glMatrixMode(GL_MODELVIEW);

	if (intersectCharacter(m))
	{
		//cout << do_once << endl;
		if (do_once)
		{
			initParticles(*(m.getPointer() + 12),*(m.getPointer() + 13)+5.0,*(m.getPointer() + 14));
			//cout << "init" << endl; 
			do_once = false;
		}
		glColor3d(1.0,0.0,0.0);
	}
	else
	{
		glColor3d(0.8,1.0,0.0);
		//do_once = true;
	}

	glLoadMatrixf(m.getPointer());
	glutSolidTeapot(1);
	if (debug)
	{
		glColor3d(1.0,0.0,0.0);
		//glutWireCube(2.2);
		drawBoundingBox();
	}
	if (intersectCharacter(m))
	{
		//glLoadIdentity();
		//updateParticles();
		//drawParticles();
	}

	updateParticles();
	drawParticles();

}