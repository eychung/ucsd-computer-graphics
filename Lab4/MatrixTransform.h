#ifndef _MATRIXTRANSFORM_H_
#define _MATRIXTRANSFORM_H_

#include "Group.h"
#include "Enum.h"
#include "Constants.h"
#include "../Matrix4.h"
#include <string.h>

class MatrixTransform : public Group {
	// should store a transformation matrix
	// that is applied to the subtree below the node
	private:
		Matrix4 m;
	
	public:
		MatrixTransform(char* nam);
		MatrixTransform(char* name, Matrix4 m);
		void addNode(Node*);
		void removeNode(Node*);
		void draw(Matrix4 c);
};

#endif