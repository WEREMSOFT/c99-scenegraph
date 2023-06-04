#include <SDL2/SDL.h>
#include "../game_t.h"
#include "../core.h"
#include "gameObject.h"

typedef enum 
{
	RUNNING_MAN_ANIMATION_RUN,
	RUNNING_MAN_ANIMATION_WALK,
	RUNNING_MAN_ANIMATION_IDLE,
	RUNING_MAN_ANIMATION_COUNT
} RunningManAnimations;

typedef struct {
	GameObject parent;
	int framesPerAnimation[RUNING_MAN_ANIMATION_COUNT];
	int framesPerSecond[RUNING_MAN_ANIMATION_COUNT];
} RunningMan;



void runningManDraw(RunningMan* _this, Game* game)
{
	_this->parent.sprite.destRect.x = (int)_this->parent.rigidBody.boundingBox.x;
	_this->parent.sprite.destRect.y = (int)_this->parent.rigidBody.boundingBox.y;
	SDL_RenderCopyEx(game->renderer, _this->parent.sprite.texture, &_this->parent.sprite.srcRect, &_this->parent.sprite.destRect, 0, NULL, _this->parent.sprite.isFlipped?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE);

	if(game->isDebugMode)
	{
		SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
		SDL_RenderDrawRectF(game->renderer, &_this->parent.rigidBody.boundingBox);
	}

}

void setAnimationIdle(RunningMan* _this)
{
	_this->parent.sprite.animation.currentAnimation = RUNNING_MAN_ANIMATION_IDLE;
	_this->parent.sprite.animation.frameCount = _this->framesPerAnimation[RUNNING_MAN_ANIMATION_IDLE];
	_this->parent.sprite.animation.framesPerSecond = _this->framesPerSecond[RUNNING_MAN_ANIMATION_IDLE];
}

void setAnimationWalk(RunningMan* _this)
{
	_this->parent.sprite.animation.currentAnimation = RUNNING_MAN_ANIMATION_WALK;
	_this->parent.sprite.animation.frameCount = _this->framesPerAnimation[RUNNING_MAN_ANIMATION_WALK];
	_this->parent.sprite.animation.framesPerSecond = _this->framesPerSecond[RUNNING_MAN_ANIMATION_WALK];
}

void setAnimationRun(RunningMan* _this)
{
	_this->parent.sprite.animation.currentAnimation = RUNNING_MAN_ANIMATION_RUN;
	_this->parent.sprite.animation.frameCount = _this->framesPerAnimation[RUNNING_MAN_ANIMATION_RUN];
	_this->parent.sprite.animation.framesPerSecond = _this->framesPerSecond[RUNNING_MAN_ANIMATION_RUN];
}

void runningManUpdate(RunningMan* _this, Game* game)
{
	processAnimationFrame(_this);

	static float runningSpeed = 250.;
	static float walkingSpeed = 100.;

	bool isMoving = false;

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
		_this->parent.rigidBody.boundingBox.x -= _this->parent.rigidBody.speed * game->deltaTime;
		_this->parent.sprite.isFlipped = true;
		isMoving = true;
	}
	if(game->keys[SDL_SCANCODE_RIGHT])
	{
		_this->parent.rigidBody.boundingBox.x += _this->parent.rigidBody.speed * game->deltaTime;
		_this->parent.sprite.isFlipped = false;
		isMoving = true;
	}
	if(game->keys[SDL_SCANCODE_UP])
	{
		_this->parent.rigidBody.boundingBox.y -= _this->parent.rigidBody.speed * game->deltaTime;
		isMoving = true;
	}
	if(game->keys[SDL_SCANCODE_DOWN])
	{
		_this->parent.rigidBody.boundingBox.y += _this->parent.rigidBody.speed * game->deltaTime;
		isMoving = true;
	}

	if(!isMoving)
	{
		setAnimationIdle(_this);
	}
}

RunningMan* runningManCreate(float position[2], SDL_Texture* texture)
{
	RunningMan* runningMan = (RunningMan*)malloc(sizeof(RunningMan));
	
	runningMan->parent.rigidBody.boundingBox.x = position[0];
	runningMan->parent.rigidBody.boundingBox.y = position[1];
	runningMan->parent.rigidBody.boundingBox.w = 25;
	runningMan->parent.rigidBody.boundingBox.h = 50;

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
	runningMan->framesPerAnimation[RUNNING_MAN_ANIMATION_IDLE] = 8;

	runningMan->framesPerSecond[RUNNING_MAN_ANIMATION_WALK] = 10;
	runningMan->framesPerSecond[RUNNING_MAN_ANIMATION_RUN] = 10;
	runningMan->framesPerSecond[RUNNING_MAN_ANIMATION_IDLE] = 5;

	runningMan->parent.sprite.animation.framesPerSecond = 10;

	runningMan->parent.sprite.center[1] = 10;

	runningMan->parent.sprite.animation.frameCount = runningMan->framesPerAnimation[RUNNING_MAN_ANIMATION_WALK];
	runningMan->parent.sprite.animation.currentAnimation = RUNNING_MAN_ANIMATION_WALK;

	return runningMan;
}
