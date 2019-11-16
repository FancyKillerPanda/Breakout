#include <string>

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
	gameOverData.gameOverText = createText(gameData.renderer, "Game Over", 64, SCREEN_WIDTH / 2, SCREEN_HEIGHT * 3 / 10);

	// Game over menu items init
	std::vector<std::string> labels = { "TRY AGAIN", "MAIN MENU", "EXIT" };
	std::vector<std::pair<int, int>> positions = {
		{ SCREEN_WIDTH / 4,     SCREEN_HEIGHT / 2 },
		{ SCREEN_WIDTH / 4 * 2, SCREEN_HEIGHT / 2 },
		{ SCREEN_WIDTH / 4 * 3, SCREEN_HEIGHT / 2 } 
	};
	gameOverData.gameOverMenu = menuConstruct(gameData.renderer, labels, positions);

	gameOverOnEnter(gameData);

	gameData.gameOverInitialised = true;
}

void gameOverOnEnter(GameData& gameData)
{
	GameOverData& gameOverData = gameData.gameOverData;
	
	// Highscore calculation
	int highScore = std::stoi(getSettingsValue(gameData.settings, "HIGH_SCORE"));

	if (highScore < gameData.score)
	{
		highScore = gameData.score;
		setSettingsValue(gameData.settings, "HIGH_SCORE", std::to_string(gameData.score).c_str());
	}

	// Score text init
	// TODO(fkp): Change font
	char tempStr[256];
	sprintf_s(tempStr, 256, "Score: %d  |  Highscore: %d", gameData.score, highScore);
	gameOverData.scoreText = createText(gameData.renderer, tempStr, 30, SCREEN_WIDTH / 2, SCREEN_HEIGHT * 7 / 10);
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
			switch (menuHandlePress(gameOverData.gameOverMenu))
			{
				case 0:
				{
					changeState(gameData, GameState::Gameplay);
				} break;
				
				case 1:
				{
					changeState(gameData, GameState::MainMenu);
				} break;
				
				case 2:
				{
					return false;
				} break;
			}
		} break;

		case SDL_KEYDOWN:
		{
			switch (gameData.event.key.keysym.sym)
			{
				case SDLK_RETURN:
				{
					switch (menuHandlePress(gameOverData.gameOverMenu))
					{
						case 0:
						{
							changeState(gameData, GameState::Gameplay);
						} break;
						
						case 1:
						{
							changeState(gameData, GameState::MainMenu);
						} break;

						case 2:
						{
							return false;
						} break;
					}
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

	drawText(gameData.renderer, gameOverData.gameOverText);
	menuDraw(gameData.renderer, gameOverData.gameOverMenu);
	drawText(gameData.renderer, gameOverData.scoreText);

	SDL_RenderPresent(gameData.renderer);
}
