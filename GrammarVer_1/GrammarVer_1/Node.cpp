#include "Node.h"



Node::~Node()
{
	delete out;
	delete in;
}
