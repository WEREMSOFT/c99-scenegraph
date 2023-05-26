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
	int framePerSecond;
	int frameCount;
	int currentFrame;
	SDL_Rect size;
} Animation;

typedef struct
{
	Node header;
	float position[2];
	float speed;
	Animation animation;
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

SDL_Rect processAnimationFrame()
{
}

#endif