#include <SDL2/SDL.h>
#include "../game_t.h"
#include "../core.h"
#include "gameObject.h"

typedef GameObject Tree;

void treeDraw(Tree* _this, SDL_Renderer* renderer)
{
	int x = (int)_this->rigidBody.position[0];
	int y = (int)_this->rigidBody.position[1];
	SDL_RenderCopyEx(renderer, _this->sprite.texture, &((SDL_Rect){0, 0, 16, 32}), &((SDL_Rect){x, y, 16, 32}), 0, NULL, SDL_FLIP_NONE);
}

void treeUpdate(Tree* _this, Game* game)
{
}

Tree* treeCreate(float position[2], float speed, SDL_Texture* texture)
{
	Tree* tree = (Tree*)malloc(sizeof(Tree));
	Node node = {0};
	tree->header = node;
	tree->rigidBody.position[0] = position[0];
	tree->rigidBody.position[1] = position[1];
	tree->update = (UpdateFunction)treeUpdate;
	tree->draw = (DrawFunction)treeDraw;
	tree->rigidBody.speed = speed;
	tree->sprite.texture = texture;
	return tree;
}
