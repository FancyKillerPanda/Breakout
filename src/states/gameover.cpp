#include <SDL/SDL.h>

#include "state.h"		

void gameoverDraw(GameData& gameData)
{
	SDL_SetRenderDrawColor(gameData.renderer, 0, 0, 0, 255);
	SDL_RenderClear(gameData.renderer);

    destroyTexture(gameData.ball.texture);
    destroyTexture(gameData.paddle.texture);
    for (Brick& brick : gameData.bricks)
    {
        destroyTexture(brick.texture);
    }
    
	SDL_RenderPresent(gameData.renderer);
}

int gameoverHandleEvents(GameData& gameData)
{
    return 0;
}