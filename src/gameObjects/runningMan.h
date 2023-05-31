#include <SDL2/SDL.h>
#include "../game_t.h"
#include "../core.h"
#include "gameObject.h"

typedef enum 
{
	RUNNING_MAN_ANIMATION_RUN,
	RUNNING_MAN_ANIMATION_WALK,
	RUNING_MAN_ANIMATION_COUNT
} RunningManAnimations;

typedef struct {
	GameObject parent;
	int framesPerAnimation[RUNING_MAN_ANIMATION_COUNT];
} RunningMan;



void runningManDraw(RunningMan* _this, SDL_Renderer* renderer)
{
	_this->parent.sprite.destRect.x = (int)_this->parent.rigidBody.position[0];
	_this->parent.sprite.destRect.y = (int)_this->parent.rigidBody.position[1];
	SDL_RenderCopyEx(renderer, _this->parent.sprite.texture, &_this->parent.sprite.srcRect, &_this->parent.sprite.destRect, 0, NULL, _this->parent.sprite.isFlipped?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE);
}

void setAnimationWalk(RunningMan* _this)
{
	_this->parent.sprite.animation.currentAnimation = RUNNING_MAN_ANIMATION_WALK;
	_this->parent.sprite.animation.frameCount = _this->framesPerAnimation[RUNNING_MAN_ANIMATION_WALK];
}

void setAnimationRun(RunningMan* _this)
{
	_this->parent.sprite.animation.currentAnimation = RUNNING_MAN_ANIMATION_RUN;
	_this->parent.sprite.animation.frameCount = _this->framesPerAnimation[RUNNING_MAN_ANIMATION_RUN];
}

void runningManUpdate(RunningMan* _this, Game* game)
{
	processAnimationFrame(_this);

	static float runningSpeed = 180.;
	static float walkingSpeed = 100.;

	if(game->keys[SDL_SCANCODE_SPACE])
	{
		setAnimationRun(_this);
		_this->parent.rigidBody.speed = runningSpeed;
	} else 
	{
		setAnimationWalk(_this);
		_this->parent.rigidBody.speed = walkingSpeed;
	}

	if(game->keys[SDL_SCANCODE_LEFT])
	{
		_this->parent.rigidBody.position[0] -= _this->parent.rigidBody.speed * game->deltaTime;
		_this->parent.sprite.isFlipped = true;
	}
	if(game->keys[SDL_SCANCODE_RIGHT])
	{
		_this->parent.rigidBody.position[0] += _this->parent.rigidBody.speed * game->deltaTime;
		_this->parent.sprite.isFlipped = false;
	}
	if(game->keys[SDL_SCANCODE_UP])
	{
		_this->parent.rigidBody.position[1] -= _this->parent.rigidBody.speed * game->deltaTime;
	}
	if(game->keys[SDL_SCANCODE_DOWN])
	{
		_this->parent.rigidBody.position[1] += _this->parent.rigidBody.speed * game->deltaTime;
	}
}

RunningMan* runningManCreate(float position[2], SDL_Texture* texture)
{
	RunningMan* runningMan = (RunningMan*)malloc(sizeof(RunningMan));
	runningMan->parent.rigidBody.position[0] = position[0];
	runningMan->parent.rigidBody.position[1] = position[1];
	runningMan->parent.update = (UpdateFunction)runningManUpdate;
	runningMan->parent.draw = (DrawFunction)runningManDraw;
	runningMan->parent.rigidBody.speed = 0;
	runningMan->parent.sprite.texture = texture;
	runningMan->parent.sprite.srcRect = (SDL_Rect){0, 0, 51, 66};
	runningMan->parent.sprite.destRect = (SDL_Rect){0, 0, 51, 66};
	runningMan->parent.sprite.animation.startTime = SDL_GetTicks();
	runningMan->parent.sprite.animation.frameCount = 6;
	runningMan->parent.sprite.animation.framesPerSecond = 10;
	runningMan->parent.sprite.animation.currentAnimation = RUNNING_MAN_ANIMATION_WALK;
	runningMan->parent.sprite.animation.frameCount = 8;

	runningMan->framesPerAnimation[RUNNING_MAN_ANIMATION_WALK] = 8;
	runningMan->framesPerAnimation[RUNNING_MAN_ANIMATION_RUN] = 6;

	runningMan->parent.sprite.center[1] = 10;

	runningMan->parent.sprite.animation.frameCount = runningMan->framesPerAnimation[RUNNING_MAN_ANIMATION_WALK];
	runningMan->parent.sprite.animation.currentAnimation = RUNNING_MAN_ANIMATION_WALK;

	return runningMan;
}
