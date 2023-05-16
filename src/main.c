#include <stdio.h>

#define __malloc__ malloc
#define __realloc__ realloc
#define __free__ free

typedef enum
{
	COMPONENT_TYPE_NONE,
	COMPONENT_TYPE_POSITION,
	COMPONENT_TYPE_RIGID_BODY,
	COMPONENT_TYPE_RENDERER,
	COMPONENT_TYPE_COUNT
} ComponentType;

struct Component;

typedef struct 
{
	ComponentType type;
	struct Component* next;
} Component;

typedef struct
{
	Component header;
	float x, y;
} ComponentPosition;

struct Node;

typedef struct
{
	struct Node* children;
	struct Node* sibling;
	Component* components;
} Node;

Node* createNode()
{
	Node* node = __malloc__(sizeof(Node));
	node->sibling = NULL;
	node->components = NULL;
	return node;
}

void nodeAddSibling(Node* _this, Node* sibling)
{
	if(_this->sibling == NULL)
	{
		_this->sibling = sibling;
		return;
	}

	Node* lastSibling = _this->sibling;
	while(lastSibling->sibling != NULL)
	{
		lastSibling = lastSibling->sibling;
	}

	lastSibling->sibling = sibling;
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

void nodeAddComponent(Node *_this, Component* component)
{
	
}

int main(void)
{
	printf("Hello World!!\n");
	return 0;
}