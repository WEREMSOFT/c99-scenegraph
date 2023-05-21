#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__
#include <SDL2/SDL.h>
#include "../core.h"
#include "../game_t.h"
#include <stddef.h>

typedef void(*UpdateFunction)(const void*, Game*);
typedef void(*DrawFunction)(const void*, const SDL_Renderer*);

typedef struct
{
	Node header;
	float position[2];
	float speed;
	SDL_Texture* texture;
	UpdateFunction update;
	DrawFunction draw;
} GameObject;


// Define a compile-time error message
struct compile_time_error;

// Perform compile-time assertion to validate signature compatibility
#define STATIC_ASSERT(expr, msg) \
    extern struct compile_time_error { int error_if_negative : (expr) ? 1 : -1; }


void gameObjectAssignUpdateFunctionCallback(GameObject* _this, UpdateFunction updateCallback)
{
	STATIC_ASSERT(sizeof(updateCallback) == sizeof(UpdateFunction), "invalid update function signature");
	_this->update = updateCallback;
}

void gameObjectAssignDrawFunctionCallback(GameObject* _this, DrawFunction drawCallback)
{
	STATIC_ASSERT(sizeof(drawCallback) == sizeof(DrawFunction), "invalid draw function signature");
	_this->update = drawCallback;
}

GameObject* gameObjectCreate()
{
	GameObject go = {0};
	GameObject* gameObject = malloc(sizeof(GameObject));
	*gameObject = go;
	return gameObject;
}

#endif