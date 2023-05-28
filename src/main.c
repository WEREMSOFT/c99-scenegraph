#include <stdio.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include "game.h"
#include "core/debugMemory.h"

int main(void)
{
	Game game = gameCreate();
	gameInit(&game);
	gameRun(&game);
	gameDestroy(game);
	return 0;
}
