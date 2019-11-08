#include <SDL/SDL.h>

#include "state.h"		

void gameOverInit(GameData& gameData)
{
	if (gameData.gameOverInitialised)
	{
		return;
	}
	
	GameOverData& gameOverData = gameData.gameOverData;
	
	// Game over text init
	gameOverData.gameOverText.text = "Game Over";
	gameOverData.gameOverText.size = 64;
	updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameOverData.gameOverText);
	gameOverData.gameOverText.rect.x = SCREEN_WIDTH / 2 - gameOverData.gameOverText.rect.w / 2;
	gameOverData.gameOverText.rect.y = SCREEN_HEIGHT * 3 / 10 - gameOverData.gameOverText.rect.h / 2;

	// Game over menu items init
	std::vector<std::string> labels = { "Retart", "Settings", "Exit" };
	std::vector<std::pair<int, int>> positions = {
		{ SCREEN_WIDTH / 4,     SCREEN_HEIGHT / 8 * 7 },
		{ SCREEN_WIDTH / 4 * 2, SCREEN_HEIGHT / 8 * 7 }, 
		{ SCREEN_WIDTH / 4 * 3, SCREEN_HEIGHT / 8 * 7 } 
	};
	gameOverData.gameOverMenu = menuConstruct(gameData.renderer, labels, positions);

	gameData.gameOverInitialised = true;
}

bool gameOverHandleEvents(GameData& gameData)
{
	if (!gameData.gameOverInitialised)
	{
		printf("Game over not initialised, cannot handle events.\n");
		return false;
	}

	GameOverData& gameOverData = gameData.gameOverData;

	// TODO(fkp): Extract into functions
	switch (gameData.event.type)
	{
		case SDL_MOUSEMOTION:
		{
			menuHandleMouseMove(gameData, gameOverData.gameOverMenu);
		} break;

		case SDL_MOUSEBUTTONDOWN:
		{
			menuHandlePress(gameOverData.gameOverMenu);
		} break;

		case SDL_KEYDOWN:
		{
			switch (gameData.event.key.keysym.sym)
			{
				case SDLK_RETURN:
				{
					menuHandlePress(gameOverData.gameOverMenu);
				} break;

				case SDLK_RIGHT:
				{
					menuHandleKeyDown(gameData, gameOverData.gameOverMenu);
				} break;

				case SDLK_LEFT:
				{
					menuHandleKeyDown(gameData, gameOverData.gameOverMenu);
				} break;
			}
		} break;
	}

	return true;
}

void gameOverDraw(GameData& gameData)
{
	if (!gameData.gameOverInitialised)
	{
		printf("Game over not initialised, cannot draw.\n");
		return;
	}

	GameOverData& gameOverData = gameData.gameOverData;

	SDL_SetRenderDrawColor(gameData.renderer, 0, 0, 0, 255);
	SDL_RenderClear(gameData.renderer);

	SDL_RenderCopy(gameData.renderer, gameData.gameOverData.gameOverText.texture, nullptr, &gameData.gameOverData.gameOverText.rect);
	menuDraw(gameData.renderer, gameOverData.gameOverMenu);

	SDL_RenderPresent(gameData.renderer);
}
