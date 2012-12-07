#include "MatrixTransform.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

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
		(*it)->draw(c_new);
	}
}