#include "Camera.h"

Camera::Camera()
{
	setE(Vector3(0,0,10));
	setD(Vector3(0,0,0));
	setUp(Vector3(0,1,0));
	updateMatrix();
}

Camera::Camera(Vector3 e, Vector3 d, Vector3 up)
{
	setE(e);
	setD(d);
	setUp(up);
	updateMatrix();
}

Matrix4 & Camera::getMatrix()
{
	return this->C;
}

Vector3 Camera::getE()
{
	return this->e;
}

Vector3 Camera::getD()
{
	return this->d;
}

Vector3 Camera::getUp()
{
	return this->up;
}

void Camera::setE(Vector3 v)
{
	this->e.set(v.get(0), v.get(1), v.get(2));
}

void Camera::setD(Vector3 v)
{
	this->d.set(v.get(0), v.get(1), v.get(2));
}

void Camera::setUp(Vector3 v)
{
	this->up.set(v.get(0), v.get(1), v.get(2));
}


void Camera::updateMatrix()
{
	this->z = (this->e - this->d).scale(1.0/((this->e - this->d).magnitude()));
	this->x = this->up.cross(up,z).scale(1.0/((this->up.cross(up,z)).magnitude()));
	this->y = z.cross(z,x);

	this->C = Matrix4(this->x.get(0), this->x.get(1), this->x.get(2), 0,
		this->y.get(0), this->y.get(1), this->y.get(2), 0,
		this->z.get(0), this->z.get(1), this->z.get(2), 0,
		this->e.get(0), this->e.get(1), this->e.get(2), 1);
}

void Camera::lookAt(Vector3 e, Vector3 d, Vector3 up)
{
	setE(e);
	setD(d);
	setUp(up);
	updateMatrix();
}

void Camera::inverseCamera()
{
	updateMatrix();

	Matrix4 r = Matrix4(this->x.get(0), this->x.get(1), this->x.get(2), 0,
		this->y.get(0), this->y.get(1), this->y.get(2), 0,
		this->z.get(0), this->z.get(1), this->z.get(2), 0,
		0, 0, 0, 1);
	r.setMatrix(r.transpose());

	Matrix4 t = Matrix4(1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		-e.get(0),-e.get(1),-e.get(2),1);
	
	this->C.setMatrix(t.multiply(r));
}

GLfloat * Camera::getGLMatrix()
{
	return this->C.getPointer();
}