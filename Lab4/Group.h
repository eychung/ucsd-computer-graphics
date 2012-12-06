// abstract subclasses Group
#ifndef _GROUP_H_
#define _GROUP_H_

#include "Node.h"
#include "../Matrix4.h"
#include <vector>

class Group : public Node {
	// store a list of pointers to child nodes
	protected:
		vector<Node*> children;

	// provide functionality to add and remove child nodes
	public:
		virtual void addNode(Node*) = 0;
		virtual void removeNode(Node*) = 0;
		virtual void draw(Matrix4 c) = 0;
};

#endif