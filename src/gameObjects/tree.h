#include <SDL2/SDL.h>
#include <math.h>
#include "../game_t.h"
#include "../core.h"
#include "gameObject.h"
typedef struct 
{
	GameObject parent;
} Tree;

void treeDraw(Tree* _this, SDL_Renderer* renderer)
{
	int x = (int)_this->parent.rigidBody.position[0];
	int y = (int)_this->parent.rigidBody.position[1];
	SDL_RenderCopyEx(renderer, _this->parent.sprite.texture, &((SDL_Rect){0, 0, 16, 32}), &((SDL_Rect){x, y, 16, 32}), 0, NULL, SDL_FLIP_NONE);
}

void treeUpdate(Tree* _this, Game* game)
{
}

Tree* treeCreate(float position[2], float speed, SDL_Texture* texture)
{
	Tree* tree = (Tree*)malloc(sizeof(Tree));
	tree->parent.rigidBody.position[0] = position[0];
	tree->parent.rigidBody.position[1] = position[1];

	tree->parent.update = (UpdateFunction)treeUpdate;
	tree->parent.draw = (DrawFunction)treeDraw;
	tree->parent.rigidBody.speed = speed;
	tree->parent.sprite.center[1] = 40;
	tree->parent.sprite.texture = texture;
	return tree;
}
