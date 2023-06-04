#include <SDL2/SDL.h>
#include <math.h>
#include "../game_t.h"
#include "../core.h"
#include "gameObject.h"
typedef struct 
{
	GameObject parent;
} StoneBlock;

void stoneBlockDraw(StoneBlock* _this, Game* game)
{
	_this->parent.sprite.destRect.x = (int)_this->parent.rigidBody.position[0];
	_this->parent.sprite.destRect.y = (int)_this->parent.rigidBody.position[1];
	SDL_RenderCopyEx(game->renderer, _this->parent.sprite.texture, &_this->parent.sprite.srcRect, &_this->parent.sprite.destRect, 0, NULL, SDL_FLIP_NONE);
}

void stoneBlockUpdate(StoneBlock* _this, Game* game)
{
}

StoneBlock* stoneBlockCreate(float position[2], SDL_Texture* texture)
{
	StoneBlock* stoneBlock = (StoneBlock*)malloc(sizeof(StoneBlock));
	stoneBlock->parent.rigidBody.position[0] = position[0];
	stoneBlock->parent.rigidBody.position[1] = position[1];

	int size[2];

	SDL_QueryTexture(texture, NULL, NULL, &size[0], &size[1]);

	stoneBlock->parent.sprite.srcRect = (SDL_Rect){0, 0, size[0], size[1]};
	stoneBlock->parent.sprite.destRect = (SDL_Rect){0, 0, size[0], size[1]};

	stoneBlock->parent.update = (UpdateFunction)stoneBlockUpdate;
	stoneBlock->parent.draw = (DrawFunction)stoneBlockDraw;
	stoneBlock->parent.sprite.center[1] = 20;
	stoneBlock->parent.sprite.texture = texture;
	return stoneBlock;
}
