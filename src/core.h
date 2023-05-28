#ifndef __CORE_H__
#define __CORE_H__
#include <stdio.h>
#include <stdlib.h>

typedef enum
{
	NODE_TYPE_NONE,
	NODE_TYPE_ROOT,
	NODE_TYPE_CHILD,
	NODE_TYPE_CHILD_2,
	NODE_TYPE_CHILD_3,
	NODE_TYPE_COUNT
} NodeType;

char* nodeTypeDescription[100] = {
	"None",
	"Root",
	"Child #1",
	"Child #2",
	"Child #3"
};

typedef enum
{
	COMPONENT_TYPE_NONE,
	COMPONENT_TYPE_POSITION,
	COMPONENT_TYPE_RIGID_BODY,
	COMPONENT_TYPE_RENDERER,
	COMPONENT_TYPE_COUNT
} ComponentType;


struct Component;
struct Node;

typedef struct 
{
	ComponentType type;
	struct Component* next;
	struct Node* parent;
} Component;

typedef struct
{
	NodeType type;
	struct Node* next;
	struct Node* prev;
	struct Node* children;
} Node;

Node* nodeCreate()
{
	Node* node = (Node*)malloc(sizeof(Node));
	node->next = NULL;
	node->children = NULL;
	return node;
}

void nodeAddSibling(Node* _this, Node* sibling)
{
	if(_this->next == NULL)
	{
		_this->next = sibling;
		sibling->prev = _this;
		return;
	}

	Node* lastSibling = _this->next;
	while(lastSibling->next != NULL)
	{
		lastSibling = lastSibling->next;
	}

	lastSibling->next = sibling;
	sibling->prev = lastSibling;
}

void nodeAddChild(Node* _this, Node* child)
{
	if(_this->children == NULL)
	{
		_this->children = child;
		return;
	}

	nodeAddSibling(_this->children, child);
}

typedef void (*TraverseNodeCallback)(Node*, void*);

void traverseGraph(Node* root, void* context, TraverseNodeCallback funPtr)
{
	if(root == NULL) return;
	Node* lastChildren = root->children;
	while(lastChildren != NULL)
	{
		Node* next = lastChildren->next;
		traverseGraph(lastChildren, context, funPtr);
		lastChildren = next;
	}
	funPtr(root, context);
}

void printNodeType(Node* node)
{
	printf("Node Type: %s\n", nodeTypeDescription[node->type]);
}

void freeNode(Node* node)
{
	free(node);
}
#endif
