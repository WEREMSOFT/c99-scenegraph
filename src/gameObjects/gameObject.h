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
	int framesPerSecond;
	int frameCount;
	int currentFrame;
	int startTime;
} Animation;

typedef struct 
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	SDL_Texture* texture;
	Animation animation;
	int center[2];
	bool isAnimated;
} Sprite;

typedef struct 
{
	float position[2];
	float initialPosition[2];
	float speed;
} RigidBody;

typedef struct
{
	Sprite sprite;
	RigidBody rigidBody;
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

Animation animationCreate(int frameCount, int framesPerSecond, bool isLoop)
{
	Animation animation = {0};
	animation.frameCount = frameCount;
	animation.framesPerSecond = framesPerSecond;
	return animation;
}

SDL_Rect processAnimationFrame(GameObject* _this)
{
	Sprite* sprite = &_this->sprite;
	Animation* animation = &sprite->animation;
	animation->currentFrame = ((SDL_GetTicks() - animation->startTime) * animation->framesPerSecond / 1000) % animation->frameCount;
	sprite->srcRect.x = animation->currentFrame * sprite->srcRect.w;
}

#endif