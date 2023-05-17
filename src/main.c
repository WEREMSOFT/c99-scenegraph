#include <stdio.h>
#include "components.h"
#include <assert.h>
#include <SDL2/SDL.h>
#include "game.h"
#include "core/debugMemory.h"

int main(void)
{
	Game game = gameCreate();
	gameInit(&game);
	gameUpdate(&game);
	gameDestroy(game);
	return 0;
}