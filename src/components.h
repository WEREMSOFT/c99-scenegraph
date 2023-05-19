#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__
#include "core/debugMemory.h"
#include "core.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct
{
	Component header;
	float x, y;
} ComponentPosition;

ComponentPosition* componentPositionCreate()
{
	ComponentPosition* component = malloc(sizeof(ComponentPosition));
	component->header.type = COMPONENT_TYPE_POSITION;
	component->header.next = NULL;
	return component;
}
#endif