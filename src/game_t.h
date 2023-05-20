#ifndef __GAME_T_H__
#define __GAME_T_H__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <assert.h>
#include "core.h"

typedef struct Game
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	SDL_Rect textureRect;
	bool keys[256];
	Node* root;
	bool isRunning;
	float deltaTime;
} Game;
#endif