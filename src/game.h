#ifndef __GAME_H__
#define __GAME_H__
#else
#error "game.h already included somewhere else"
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <assert.h>
#include "core.h"
#include "gameObjects/gameObject.h"
#include "gameObjects/tree.h"
#include "gameObjects/runningMan.h"
#include "gameObjects/frameCounter.h"
#include "game_t.h"

#define MAX_NODES 100000

Node nodes[MAX_NODES] = {0};
int nodesCount = 0;

Node* getFreeNode()
{
	assert(nodesCount+1 < MAX_NODES && "Max amount of nodes reached");
	return &nodes[nodesCount++];
}

void updateComponentCallback(Node* node, Game* game)
{
	GameObject* gameObject = (GameObject*)node->data;
	if(gameObject != NULL && gameObject->update != NULL)
		gameObject->update(gameObject, game);
}

void renderComponentCallback(Node* node, Game* game)
{
	GameObject* gameObject = (GameObject*)node->data;
	if(gameObject != NULL && gameObject->draw != NULL)
		gameObject->draw(gameObject, game);
}

Game gameCreate()
{
	Game game = {0};
	game.isRunning = true;

	game.screenSize[0] = 800;
	game.screenSize[1] = 600;

	assert(SDL_Init(SDL_INIT_EVERYTHING) == 0 && "Error initializing SDL\n");

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);

	game.window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		game.screenSize[0],
		game.screenSize[1],
		0);

	assert(game.window != NULL && "Error creating SDL window");

 	game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	assert(game.renderer != NULL && "Error creating renderer");

	game.assetManager = assetManagerInit(game.renderer);

	return game;
}

RunningMan* hero;

void gameInit(Game *game)
{
	nodes[nodesCount++].data = gameObjectCreate();
	game->root = &nodes[nodesCount-1];

	int padding[2] = { game->screenSize[0] / 10, game->screenSize[1] / 10};
	for(int i = 0; i < 11; i++)
		for(int j = 0; j < 11; j++)
		{
			Node* node = getFreeNode();
			node->data = treeCreate((float[]){i * padding[0] - 50., j * padding[1] - 50.}, 100., game->assetManager.textures[ASSET_IMAGE_TREE]);
			nodeAddChild(game->root, node);
		}

	{
		Node* node = getFreeNode();
		node->data = runningManCreate((float[]){0, 0}, game->assetManager.textures[ASSET_IMAGE_RUNNING_MAN]);
		hero = node->data;
		nodeAddChild(game->root, node);
	}
	{
		Node *node = getFreeNode();
		node->data = frameCounterCreate(game->assetManager.fonts[ASSET_FONT_CHARRIOT]);
		nodeAddChild(game->root, node);
	}
}

void swapData(Node* a, Node* b) {
    void *temp = a->data;
    a->data = b->data;
    b->data = temp;
}

// Comparator function to compare two nodes
typedef int (*Comparator)(const void*, const void*);

// Function to sort the linked list using a comparator function pointer
void sortLinkedList(Node* head, Comparator compare) {
    if (head == NULL || head->next == NULL)
        return;

    int swapped;
    Node* ptr1;
    Node* lptr = NULL;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != NULL) {
			Node* nn = ptr1->next;
            if (compare(ptr1->data, nn->data) > 0) {
                swapData(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

int compareGo(GameObject* a, GameObject* b)
{
	return (a->rigidBody.position[1] - a->sprite.center[1]) - (b->rigidBody.position[1] - b->sprite.center[1]);
}

SDL_Rect testRect = {100, 100, 100, 100};

void gameRender(Game game)
{
	sortLinkedList(game.root->children, compareGo);

	SDL_SetRenderDrawColor(game.renderer, 21, 21, 21, 255);
	SDL_RenderClear(game.renderer);
	traverseGraph(game.root, &game, renderComponentCallback);

	SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(game.renderer, &testRect);

	SDL_Rect heroRect = hero->parent.sprite.destRect;
	heroRect.x = hero->parent.rigidBody.position[0];
	heroRect.y = hero->parent.rigidBody.position[1];

	SDL_Rect result;

	if(SDL_IntersectFRect(&testRect, &heroRect, &result))
	{
		SDL_RenderFillRect(game.renderer, &result);
		if(result.w < result.h)
		{
			if(result.x < hero->parent.rigidBody.position[0])
				hero->parent.rigidBody.position[0] += result.w;
			else
				hero->parent.rigidBody.position[0] -= result.w;
		} else 
		{
			if(result.y < hero->parent.rigidBody.position[1])
				hero->parent.rigidBody.position[1] += result.h;
			else
				hero->parent.rigidBody.position[1] -= result.h;
		}
	}

	SDL_RenderPresent(game.renderer);
}


int checkAABBCollision(SDL_Rect* rect1, SDL_Rect* rect2, int fixRect1) {
    int collision = 0;

    if (SDL_HasIntersection(rect1, rect2)) {
        collision = 1;

        if (!fixRect1) {
            // Adjust rect1 to resolve collision
            if (rect1->x + rect1->w > rect2->x && rect1->x < rect2->x + rect2->w) {
                if (rect1->y + rect1->h > rect2->y && rect1->y < rect2->y + rect2->h) {
                    int dx = (rect1->x + rect1->w / 2) - (rect2->x + rect2->w / 2);
                    int dy = (rect1->y + rect1->h / 2) - (rect2->y + rect2->h / 2);

                    if (abs(dx) > abs(dy)) {
                        if (dx > 0) {
                            rect1->x = rect2->x + rect2->w;
                        } else {
                            rect1->x = rect2->x - rect1->w;
                        }
                    } else {
                        if (dy > 0) {
                            rect1->y = rect2->y + rect2->h;
                        } else {
                            rect1->y = rect2->y - rect1->h;
                        }
                    }
                }
            }
        } else {
            // Adjust rect2 to resolve collision
            if (rect2->x + rect2->w > rect1->x && rect2->x < rect1->x + rect1->w) {
                if (rect2->y + rect2->h > rect1->y && rect2->y < rect1->y + rect1->h) {
                    int dx = (rect2->x + rect2->w / 2) - (rect1->x + rect1->w / 2);
                    int dy = (rect2->y + rect2->h / 2) - (rect1->y + rect1->h / 2);

                    if (abs(dx) > abs(dy)) {
                        if (dx > 0) {
                            rect2->x = rect1->x + rect1->w;
                        } else {
                            rect2->x = rect1->x - rect2->w;
                        }
                    } else {
                        if (dy > 0) {
                            rect2->y = rect1->y + rect1->h;
                        } else {
                            rect2->y = rect1->y - rect2->h;
                        }
                    }
                }
            }
        }
    }

    return collision;
}

void gameRun(Game* game)
{
	float lastFrameTime = SDL_GetTicks();
	while(game->isRunning)
	{
		game->deltaTime = (SDL_GetTicks() - lastFrameTime) / 1000. ;
		lastFrameTime = SDL_GetTicks();
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			// Handle SDL events
			switch (event.type)
			{
				case SDL_QUIT:
					game->isRunning = false;
					break;
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE)
					{
						game->isRunning = false;
						break;
					}
					
					game->keys[event.key.keysym.scancode] = true;
					break;
				case SDL_KEYUP:
					game->keys[event.key.keysym.scancode] = false;
					if(event.key.keysym.sym == SDLK_1)
					{
						game->isDebugMode = !game->isDebugMode;
						break;
					}
					break;
			}
		}
		traverseGraph(game->root, game, (TraverseNodeCallback)updateComponentCallback);

		SDL_Rect heroRect = hero->parent.sprite.destRect;

		heroRect.x = hero->parent.rigidBody.position[0];
		heroRect.y = hero->parent.rigidBody.position[1];
						
		gameRender(*game);
	}
}

void gameDestroy(Game game)
{
	traverseGraph(game.root, &game, freeNode);
	assetManagerDestroy(game.assetManager);
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();
}
