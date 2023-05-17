#ifndef __CORE_H__
#define __CORE_H__
#include <stdio.h>
#include <stdlib.h>

#define __malloc__ malloc
#define __realloc__ realloc
#define __free__ free

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
	struct Node* children;
	Component* components;
} Node;

Node* nodeCreate()
{
	Node* node = __malloc__(sizeof(Node));
	node->components = NULL;
	node->next = NULL;
	node->children = NULL;
	return node;
}

void nodeAddComponent(Node* _this, Component* component)
{
	if(_this->components == NULL)
	{
		component->parent = _this;
		_this->components = component;
		return;
	}

	Component* lastComponent = _this->components;
	while(lastComponent->next != NULL)
	{
		lastComponent = lastComponent->next;
	}
	lastComponent->next = component;
}

void nodeAddSibling(Node* _this, Node* sibling)
{
	if(_this->next == NULL)
	{
		_this->next = sibling;
		return;
	}

	Node* lastSibling = _this->next;
	while(lastSibling->next != NULL)
	{
		lastSibling = lastSibling->next;
	}

	lastSibling->next = sibling;
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

void traverseGraph(Node* root, void (*funPtr)(Node*))
{
	if(root == NULL) return;
	Node* lastChildren = root->children;
	while(lastChildren != NULL)
	{
		Node* next = lastChildren->next;
		traverseGraph(lastChildren, funPtr);
		lastChildren = next;
	}
	funPtr(root);
}

void printNodeType(Node* node)
{
	printf("Node Type: %s\n", nodeTypeDescription[node->type]);
}

void freeNode(Node* node)
{
	Component* component = node->components;
	while(component != NULL)
	{
		Component* next = component->next;
		__free__(component);
		component = next;
	}
	__free__(node);
}
#endif
