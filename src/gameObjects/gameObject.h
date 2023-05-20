#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__
#include <SDL2/SDL.h>
#include "../core.h"
#include "../game_t.h"

typedef void(*UpdateFunction)(void*, Game*);
typedef void(*DrawFunction)(void*, SDL_Renderer*);

typedef struct
{
	Node header;
	float position[2];
	float speed;
	SDL_Texture* texture;
	UpdateFunction update;
	DrawFunction draw;
} GameObject;

GameObject* gameObjectCreate()
{
	GameObject go = {0};
	GameObject* gameObject = malloc(sizeof(GameObject));
	*gameObject = go;
	return gameObject;
}

#endif