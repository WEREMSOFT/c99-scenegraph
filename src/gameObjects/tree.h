#include <SDL2/SDL.h>
#include "../game_t.h"
#include "../core.h"
#include "gameObject.h"

typedef GameObject Tree;

void treeDraw(Tree* _this, SDL_Renderer* renderer)
{
	int x = (int)_this->position[0];
	int y = (int)_this->position[1];
	SDL_RenderCopyEx(renderer, _this->texture, &((SDL_Rect){0, 0, 16, 32}), &((SDL_Rect){x, y, 16, 32}), 0, NULL, SDL_FLIP_NONE);
}

void treeUpdate(Tree* _this, Game* game)
{
	if(game->keys[SDL_SCANCODE_LEFT])
	{
		_this->position[0] -= _this->speed * game->deltaTime;
	}
	if(game->keys[SDL_SCANCODE_RIGHT])
	{
		_this->position[0] += _this->speed * game->deltaTime;
	}
	if(game->keys[SDL_SCANCODE_UP])
	{
		_this->position[1] -= _this->speed * game->deltaTime;
	}
	if(game->keys[SDL_SCANCODE_DOWN])
	{
		_this->position[1] += _this->speed * game->deltaTime;
	}
}

Tree* treeCreate(float position[2], float speed, SDL_Texture* texture)
{
	Tree* tree = (Tree*)malloc(sizeof(Tree));
	Node node = {0};
	tree->header = node;
	tree->position[0] = position[0];
	tree->position[1] = position[1];
	tree->update = (UpdateFunction)treeUpdate;
	tree->draw = (DrawFunction)treeDraw;
	tree->speed = speed;
	tree->texture = texture;
	return tree;
}