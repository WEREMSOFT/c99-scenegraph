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
#include "gameObjects/frameCounter.h"
#include "game_t.h"

void updateComponentCallback(Node* node, Game* game)
{
	GameObject* gameObject = (GameObject*)node;
	if(gameObject->update != NULL)
		gameObject->update(gameObject, game);
}

void renderComponentCallback(Node* node, Game* game)
{
	GameObject* gameObject = (GameObject*)node;
	if(gameObject->draw != NULL)
		gameObject->draw(gameObject, game->renderer);
}

Game gameCreate()
{
	Game game = {0};
	game.isRunning = true;

	assert(SDL_Init(SDL_INIT_EVERYTHING) == 0 && "Error initializing SDL\n");

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);

	game.window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		0);

	assert(game.window != NULL && "Error creating SDL window");

 	game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	assert(game.renderer != NULL && "Error creating renderer");

	game.assetManager = assetManagerInit(game.renderer);

	return game;
}

void fakeFunction(void)
{
	printf("this is a test/n");
}

void gameInit(Game *game)
{
	game->root = (Node*)gameObjectCreate();
	game->root->type = NODE_TYPE_ROOT;

	{
		Tree* child = treeCreate((float[]){0, 0}, 100., game->assetManager.textures[ASSET_IMAGE_TREE]);
		child->header.type = NODE_TYPE_CHILD;
		nodeAddChild(game->root, (Node*)child);
	}
	{
		FrameCounter* fc = frameCounterCreate(game->assetManager.fonts[ASSET_FONT_CHARRIOT]);
		nodeAddChild(game->root, (Node*)fc);
	}
}

void gameRender(Game game)
{
	SDL_SetRenderDrawColor(game.renderer, 21, 21, 21, 255);
	SDL_RenderClear(game.renderer);
	traverseGraph(game.root, &game, renderComponentCallback);
	SDL_RenderPresent(game.renderer);
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
					break;
			}
		}
		traverseGraph(game->root, game, (TraverseNodeCallback)updateComponentCallback);
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