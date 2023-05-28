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
	bool isAnimated;
} Sprite;

typedef struct 
{
	float position[2];
	float speed;
} RigidBody;

typedef struct
{
	Node header;
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

// Sort linked list of childs
typedef int (*Comparator)(const void*, const void*);

void swapData(GameObject* a, GameObject* b) {
    void* temp = a->header.next;

	GameObject* prevA = a->header.prev;
	GameObject* prevB = b->header.prev;

	if(prevA != NULL)
	{
		prevA->header.next = b;
		b->header.prev = prevA;
	}
	if(prevB != NULL)
	{
		prevB->header.next = a;
		a->header.prev = prevB;
	}
}

void sortLinkedList(GameObject** head, Comparator compare) {
    if (*head == NULL || (*head)->header.next == NULL)
        return;

    int swapped;
    GameObject* ptr1;
    GameObject* lptr = NULL;

    do {
        swapped = 0;
        ptr1 = *head;
        while (ptr1 != NULL && ptr1->header.next != NULL) {
            if (compare(ptr1, ptr1->header.next) > 0) {
                swapData(ptr1, ptr1->header.next);
                swapped = 1;
            }
            ptr1 = ptr1->header.next;
        }
        lptr = ptr1;
    } while (swapped);
}

#endif