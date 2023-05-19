#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <assert.h>
#include "core.h"
#include "gameObjects/gameObject.h"
#include "gameObjects/tree.h"

typedef struct Game
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Rect textureRect;
	Node* root;
	bool isRunning;
} Game;

void updateComponentCallback(Node* node, Game* game)
{
	GameObject* gameObject = (GameObject*)node;
	if(gameObject->update != NULL)
		gameObject->update(gameObject, 1.);
}

void nodeComponentUpdate(Node* node, void* game)
{
	traverseGraph(node, game, (TraverseNodeCallback)updateComponentCallback);
}

void renderComponentCallback(Node* node, Game* game)
{
	GameObject* gameObject = (GameObject*)node;
	if(gameObject->draw != NULL)
		gameObject->draw(gameObject, game->renderer);
}

void gameObjectRender(Node* node, void* game)
{
	traverseGraph(node, game, (TraverseNodeCallback)renderComponentCallback);
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
	

	SDL_Surface* surface = IMG_Load("assets/tree.png");
	game.texture = SDL_CreateTextureFromSurface(game.renderer, surface);
	game.textureRect = (SDL_Rect){0, 0, surface->w, surface->h};
	SDL_FreeSurface(surface);

	return game;
}

void gameRender(Game game)
{
	SDL_SetRenderDrawColor(game.renderer, 21, 21, 21, 255);
	SDL_RenderClear(game.renderer);
	
	traverseGraph(game.root, &game, gameObjectRender);

	SDL_RenderPresent(game.renderer);
}

void gameInit(Game *game)
{
	game->root = (Node*)gameObjectCreate();
	game->root->type = NODE_TYPE_ROOT;
	{
		Tree* child = treeCreate((float[]){0, 0}, 1., game->texture);
		child->header.type = NODE_TYPE_CHILD;
		nodeAddChild(game->root, (Node*)child);
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
		nodeComponentUpdate(game->root, game);
		gameRender(*game);
	}
}

void gameDestroy(Game game)
{
	traverseGraph(game.root, &game, freeNode);
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();
}