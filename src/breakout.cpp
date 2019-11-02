#include <stdio.h>
#include <math.h>

#include <SDL/SDL.h>

#include "breakout.h"
#include "entity.h"
#include "states/state.h"

// NOTE(fkp): Returns true on success, false on fail
GameData init()
{
	GameData result = {};
	result.keyboardState = SDL_GetKeyboardState(NULL);
	result.running = true;  // Set to false if init fails
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		printf("Failed to Initialise SDL2\n");
		result.running = false;
	}

	if (TTF_Init() != 0)
	{
		printf("Failed to initialise TTF\n");
		result.running = false;
	}

	result.window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	if (result.window == nullptr)
	{
		printf("Failed to Create Window\n");
		result.running = false;
	}

	result.renderer = SDL_CreateRenderer(result.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (result.renderer == nullptr)
	{
		printf("Failed to Create Renderer\n");
		result.running = false;
	}

	// Sets the logical size that will be used for the game
	SDL_RenderSetLogicalSize(result.renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Sets the blend mode
	SDL_SetRenderDrawBlendMode(result.renderer, SDL_BLENDMODE_BLEND);

	return result;
}

// TODO(lucky962): Should this be separate for each game state?
bool gameInit(GameData& gameData)
{
	// Text init
	gameData.fpsText.text = "Breakout V0.1.0 | 0.00 FPS";
	gameData.fpsText.rect.x = SCREEN_WIDTH * 4 / 5;
	gameData.fpsText.rect.y = SCREEN_HEIGHT * 15 / 16;
	updateTextTexture(gameData.renderer, ARIAL_FONT_PATH, gameData.fpsText);
	
	gameData.pausedText.text = "PAUSED";
	gameData.pausedText.colour = SDL_Colour { 255, 0, 0, 255 };
	gameData.pausedText.size = 64;
	updateTextTexture(gameData.renderer, BAD_SIGNAL_FONT_PATH, gameData.pausedText);
	gameData.pausedText.rect.x = (SCREEN_WIDTH - gameData.pausedText.rect.w) / 2;
	gameData.pausedText.rect.y = (SCREEN_HEIGHT - gameData.pausedText.rect.h) / 2;

	paddleReset(gameData.renderer, gameData.paddle);
	ballReset(gameData.renderer, gameData.ball);
	bricksReset(gameData);
	
	// Initialises Menu
	initMenu(gameData);

	return true;
}

// NOTE(fkp): Returns true if success, false if games needs to exit
// This function is for common events
bool gameHandleEvents(GameData& gameData)
{
	while (SDL_PollEvent(&gameData.event))
	{
		switch (gameData.event.type)
		{
			case SDL_QUIT:
			{
				return false;
			} break;

			case SDL_KEYDOWN:
			{
				switch (gameData.event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
					{
						if (gameData.gameState != GameState::Gameplay)
						{
							return false;
						}
					} break;

					case SDLK_F11:
					{
						if (gameData.fullscreen)
						{
							SDL_SetWindowFullscreen(gameData.window, 0);
						}
						else
						{
							SDL_SetWindowFullscreen(gameData.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
						}
						
						gameData.fullscreen = !gameData.fullscreen;
					} break;
				}
			} break;
		}

		switch (gameData.gameState)
		{
			case GameState::Gameplay:
			{
				if (!gameplayHandleEvents(gameData))
				{
					return false;
				}
			} break;
			
			case GameState::Menu:
			{
				switch(menuHandleEvents(gameData))
				{
					case MenuButtonSelected::Start:
					{
						gameData.gameState = GameState::Gameplay;
					} break;

					case MenuButtonSelected::Customise:
					{
						gameData.menuState = MenuState::Customise;
					} break;

					case MenuButtonSelected::Exit:
					{
						return false;
					} break;
					
				}
			} break;
		}
	}

	return true;
}
