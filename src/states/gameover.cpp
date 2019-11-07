#include <SDL/SDL.h>

#include "state.h"		

void gameOverInit(GameData& gameData)
{
    if (gameData.gameOverInitialised)
    {
        return;
    }
    
    GameOverData& gameOverData = gameData.gameOverData;
    
    gameOverData.gameOverText.text = "Game Over";
    gameOverData.gameOverText.size = 50;
    updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameOverData.gameOverText);
    gameOverData.gameOverText.rect.x = SCREEN_WIDTH * 1 / 2 - gameOverData.gameOverText.rect.w / 2;
    gameOverData.gameOverText.rect.y = SCREEN_HEIGHT * 1 / 2 - gameOverData.gameOverText.rect.h / 2;

    gameData.gameOverInitialised = true;
}

bool gameOverHandleEvents(GameData& gameData)
{
    if (!gameData.gameOverInitialised)
    {
        printf("Game over not initialised, cannot handle events.\n");
        return false;
    }
}

void gameoverDraw(GameData& gameData)
{
    if (!gameData.gameOverInitialised)
    {
        printf("Game over not initialised, cannot draw.\n");
        return;
    }

	SDL_SetRenderDrawColor(gameData.renderer, 0, 0, 0, 255);
	SDL_RenderClear(gameData.renderer);

    destroyTexture(gameData.ball.texture);
    destroyTexture(gameData.paddle.texture);
    for (Brick& brick : gameData.bricks)
    {
        destroyTexture(brick.texture);
    }

	SDL_RenderCopy(gameData.renderer, gameData.gameOverData.gameOverText.texture, nullptr, &gameData.gameOverData.gameOverText.rect);
	SDL_RenderPresent(gameData.renderer);
}
