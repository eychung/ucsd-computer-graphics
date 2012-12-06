#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

#include "lib/glut-3.7.6-bin/include/GL/glut.h"

class Camera
{
	protected:
		Vector3 e;
		Vector3 d;
		Vector3 up;
		Vector3 x, y, z;
		Matrix4 C;

	public:
		Camera();
		Camera(Vector3 e, Vector3 d, Vector3 up);

		Matrix4 & getMatrix();
		Vector3 getE();
		Vector3 getD();
		Vector3 getUp();
		void setE(Vector3);
		void setD(Vector3);
		void setUp(Vector3);

		void updateMatrix();
		void lookAt(Vector3 e, Vector3 d, Vector3 up);
		void inverseCamera();

		GLfloat * getGLMatrix();

};

#endif