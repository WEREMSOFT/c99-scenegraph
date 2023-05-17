#include <SDL2/SDL.h>
#include <stdbool.h>
#include <assert.h>
#include "core.h"

typedef struct Game
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	Node* root;
	bool isRunning;
} Game;

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

	return game;
}

void gameRender(Game game)
{
	SDL_SetRenderDrawColor(game.renderer, 21, 21, 21, 255);
	SDL_RenderClear(game.renderer);
	
	SDL_RenderPresent(game.renderer);
}

void gameInit(Game *game)
{
	game->root = nodeCreate();
	game->root->type = NODE_TYPE_ROOT;
	{
		Node* child = nodeCreate();
		child->type = NODE_TYPE_CHILD;
		nodeAddChild(game->root, child);
	}
	{
		Node *child = nodeCreate();
		child->type = NODE_TYPE_CHILD;
		nodeAddChild(game->root, child);
		Node* child2 = nodeCreate();
		child2->type = NODE_TYPE_CHILD_2;
		nodeAddChild(child, child2);
	}
	{
		Component* component = componentPositionCreate();
		nodeAddComponent(game->root, component);
	}
}

void gameUpdate(Game* game)
{
	while(game->isRunning)
	{
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
						game->isRunning = false;

			}
		}
		nodeComponentUpdate(game->root);
		traverseGraph(game->root, printNodeType);
		gameRender(*game);
	}
}

void gameDestroy(Game game)
{
	traverseGraph(game.root, freeNode);
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();
}