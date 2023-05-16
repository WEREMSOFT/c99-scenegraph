#include <stdio.h>
#include "core.h"
#include "components.h"
int main(void)
{
	Node* root = nodeCreate();
	root->type = NODE_TYPE_ROOT;
	Node* child = nodeCreate();
	child->type = NODE_TYPE_CHILD;
	nodeAddChild(root, child);

	child = nodeCreate();
	child->type = NODE_TYPE_CHILD;
	nodeAddChild(root, child);

	Node* child2 = nodeCreate();
	child2->type = NODE_TYPE_CHILD_2;
	nodeAddChild(child, child2);

	traverseGraph(root, printNodeType);
	traverseGraph(root, freeNode);
	printf("Hello World!!\n");
	return 0;
}