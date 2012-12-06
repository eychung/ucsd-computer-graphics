#include "MatrixTransform.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

extern double r_torso;
extern double r_head;
extern double r_shoulder_left;
extern double r_shoulder_right;
extern double r_leg_left;
extern double r_leg_right;
extern double r_arm_left;
extern double r_arm_right;
extern double r_foot_left;
extern double r_foot_right;

extern bool b_torso;
extern bool b_head_forward;
extern bool b_shoulder_left_forward;
extern bool b_shoulder_right_forward;
extern bool b_leg_left_forward;
extern bool b_leg_right_forward;
extern bool b_arm_left_forward;
extern bool b_arm_right_forward;
extern bool b_foot_left_forward;
extern bool b_foot_right_forward;

extern float running;

extern bool animate;

MatrixTransform::MatrixTransform(char* name)
{
	this->name = name;
}

MatrixTransform::MatrixTransform(char* name, Matrix4 m)
{
	this->name = name;
	this->m = m;
}

void MatrixTransform::addNode(Node* n)
{
	children.push_back(n);
}

void MatrixTransform::removeNode(Node* n)
{
	vector<Node*>::iterator it;
	for (it = children.begin(); it != children.end(); it++)
	{
		if (strcmp(this->name,n->getName()) == 0)
		{
			children.erase(it);
			break;
		}
	}
}

void MatrixTransform::draw(Matrix4 c)
{
	Matrix4 c_new = m.multiply(c);
	vector<Node*>::iterator it;
	for (it = children.begin(); it != children.end(); it++)
	{
		if (animate)
		{
			if (strcmp(this->name,id_running) == 0)
			{
				(*it)->draw((c_new.rotateY(running)).multiply(c_new));
				running += 0.01f;
			}
			else if (strcmp(this->name,id_torso) == 0)
			{
				(*it)->draw((c_new.rotateY(r_torso)).multiply(c_new));
				if (b_torso)
				{
					r_torso -= 0.001;
					if (r_torso < -0.5)
					{
						b_torso = false;
					}
				}
				else
				{
					r_torso += 0.001;
					if (r_torso > 0.5)
					{
						b_torso = true;
					}
				}
			}
			else if (strcmp(this->name,id_head) == 0)
			{
				(*it)->draw((c_new.rotateZ(r_head)).multiply(c_new));
				if (b_head_forward)
				{
					r_head -= 0.002;
					if (r_head < -0.3)
					{
						b_head_forward = false;
					}
				}
				else
				{
					r_head += 0.002;
					if (r_head > 0.3)
					{
						b_head_forward = true;
					}
				}
			}
			else if (strcmp(this->name,id_shoulder_left) == 0)
			{
				(*it)->draw((c_new.rotateX(r_shoulder_left)).multiply(c_new));
				if (b_shoulder_left_forward)
				{
					r_shoulder_left -= 0.002;
					if (r_shoulder_left < -0.8)
					{
						b_shoulder_left_forward = false;
					}
				}
				else
				{
					r_shoulder_left += 0.002;
					if (r_shoulder_left > 0.8)
					{
						b_shoulder_left_forward = true;
					}
				}
			}
			else if (strcmp(this->name,id_shoulder_right) == 0)
			{
				(*it)->draw((c_new.rotateX(r_shoulder_right)).multiply(c_new));
				if (b_shoulder_right_forward)
				{
					r_shoulder_right -= 0.002;
					if (r_shoulder_right < -0.8)
					{
						b_shoulder_right_forward = false;
					}
				}
				else
				{
					r_shoulder_right += 0.002;
					if (r_shoulder_right > 0.8)
					{
						b_shoulder_right_forward = true;
					}
				}
			}
			else if (strcmp(this->name,id_leg_left) == 0)
			{
				(*it)->draw((c_new.rotateX(r_leg_left)).multiply(c_new));
				if (b_leg_left_forward)
				{
					r_leg_left -= 0.005;
					if (r_leg_left < -0.5)
					{
						b_leg_left_forward = false;
					}
				}
				else
				{
					r_leg_left += 0.005;
					if (r_leg_left > 0.5)
					{
						b_leg_left_forward = true;
					}
				}
			}
			else if (strcmp(this->name,id_leg_right) == 0)
			{
				(*it)->draw((c_new.rotateX(r_leg_right)).multiply(c_new));
				if (b_leg_right_forward)
				{
					r_leg_right -= 0.005;
					if (r_leg_right < -0.5)
					{
						b_leg_right_forward = false;
					}
				}
				else
				{
					r_leg_right += 0.005;
					if (r_leg_right > 0.5)
					{
						b_leg_right_forward = true;
					}
				}
			}
			else if (strcmp(this->name,id_arm_left) == 0)
			{
				(*it)->draw((c_new.rotateX(r_arm_left)).multiply(c_new));
				if (b_arm_left_forward)
				{
					r_arm_left -= 0.005;
					if (r_arm_left < -0.5)
					{
						b_arm_left_forward = false;
					}
				}
				else
				{
					r_arm_left += 0.005;
					if (r_arm_left > 0.5)
					{
						b_arm_left_forward = true;
					}
				}
			}
			else if (strcmp(this->name,id_arm_right) == 0)
			{
				(*it)->draw((c_new.rotateX(r_arm_right)).multiply(c_new));
				if (b_arm_right_forward)
				{
					r_arm_right -= 0.005;
					if (r_arm_right < -0.5)
					{
						b_arm_right_forward = false;
					}
				}
				else
				{
					r_arm_right += 0.005;
					if (r_arm_right > 0.5)
					{
						b_arm_right_forward = true;
					}
				}
			}
			else if (strcmp(this->name,id_foot_left) == 0)
			{
				(*it)->draw((c_new.rotateZ(r_foot_left)).multiply(c_new));
				r_foot_left += 0.03;
			}
			else if (strcmp(this->name,id_foot_right) == 0)
			{
				(*it)->draw((c_new.rotateZ(r_foot_right)).multiply(c_new));
				r_foot_right -= 0.03;
			}
			else
			{
				(*it)->draw(c_new);
			}
		}
		else
		{
			(*it)->draw(c_new);
		}
	}
}