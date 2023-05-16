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
}

#endif