// abstract base class Node for nodes in the scene graph
#ifndef _NODE_H_
#define _NODE_H_

#include "../Matrix4.h"

using namespace std;

class Node {
	protected:
		char* name;
		Node* parent;
		// bounding box
	public:
		char* getName() { return name; }
		virtual void draw(Matrix4 m) = 0;
};

#endif