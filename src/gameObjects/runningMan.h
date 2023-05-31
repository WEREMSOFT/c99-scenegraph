#include <SDL2/SDL.h>
#include "../game_t.h"
#include "../core.h"
#include "gameObject.h"

typedef GameObject RunningMan;

void runningManDraw(RunningMan* _this, SDL_Renderer* renderer)
{
	_this->sprite.destRect.x = (int)_this->rigidBody.position[0];
	_this->sprite.destRect.y = (int)_this->rigidBody.position[1];
	SDL_RenderCopyEx(renderer, _this->sprite.texture, &_this->sprite.srcRect, &_this->sprite.destRect, 0, NULL, _this->sprite.isFlipped?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE);
}

void runningManUpdate(RunningMan* _this, Game* game)
{
	processAnimationFrame(_this);
	if(game->keys[SDL_SCANCODE_LEFT])
	{
		_this->rigidBody.position[0] -= _this->rigidBody.speed * game->deltaTime;
		_this->sprite.isFlipped = true;
		// _this->sprite.srcRect.y = _this->sprite.srcRect.h * 3;
	}
	if(game->keys[SDL_SCANCODE_RIGHT])
	{
		_this->rigidBody.position[0] += _this->rigidBody.speed * game->deltaTime;
		_this->sprite.isFlipped = false;
		// _this->sprite.srcRect.y = _this->sprite.srcRect.h;
	}
	if(game->keys[SDL_SCANCODE_UP])
	{
		_this->rigidBody.position[1] -= _this->rigidBody.speed * game->deltaTime;
		// _this->sprite.srcRect.y = 0;
	}
	if(game->keys[SDL_SCANCODE_DOWN])
	{
		_this->rigidBody.position[1] += _this->rigidBody.speed * game->deltaTime;
		// _this->sprite.srcRect.y = _this->sprite.srcRect.h * 2;
	}
}

RunningMan* runningManCreate(float position[2], float speed, SDL_Texture* texture)
{
	RunningMan* runningMan = (RunningMan*)malloc(sizeof(RunningMan));
	runningMan->rigidBody.position[0] = position[0];
	runningMan->rigidBody.position[1] = position[1];
	runningMan->update = (UpdateFunction)runningManUpdate;
	runningMan->draw = (DrawFunction)runningManDraw;
	runningMan->rigidBody.speed = speed;
	runningMan->sprite.texture = texture;
	runningMan->sprite.srcRect = (SDL_Rect){0, 0, 51, 66};
	runningMan->sprite.destRect = (SDL_Rect){0, 0, 51, 66};
	runningMan->sprite.animation.startTime = SDL_GetTicks();
	runningMan->sprite.animation.frameCount = 6;
	runningMan->sprite.animation.framesPerSecond = 10;
	return runningMan;
}
