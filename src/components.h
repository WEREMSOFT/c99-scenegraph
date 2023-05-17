#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__
#include "core.h"

typedef struct
{
	Component header;
	float x, y;
} ComponentPosition;


ComponentPosition* componentPositionCreate()
{
	ComponentPosition* component = __malloc__(sizeof(ComponentPosition));
	component->header.type = COMPONENT_TYPE_POSITION;
	component->header.next = NULL;
	return component;
}

void updateComponentCallback(Node* node)
{
	Component* component = node->components;
	while(component != NULL)
	{
		printf("component type %d\n", component->type);
		component = component->next;
	}
}

void nodeComponentUpdate(Node* node, float deltaTime)
{
	traverseGraph(node, updateComponentCallback);
}
#endif