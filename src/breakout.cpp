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
	gameData.settings = loadSettings();
	
	// FPS text init
	gameData.fpsText = createText(gameData.renderer, "Breakout V0.1.0 | 0.00 FPS", 14, SCREEN_WIDTH * 9 / 10, SCREEN_HEIGHT * 31 / 32, ARIAL_FONT_PATH, SDL_Color { 255, 255, 255, 255 });
	
	// Initialises first state
	mainMenuInit(gameData);

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
			
			case GameState::MainMenu:
			{
				switch(menuHandleEvents(gameData))
				{
					case MenuButtonSelected::Start:
					{
						changeState(gameData, GameState::Gameplay);
					} break;

					case MenuButtonSelected::Customise:
					{
						gameData.mainMenuData.state = MainMenuState::Customise;
					} break;

					case MenuButtonSelected::Exit:
					{
						return false;
					} break;
					
				}
			} break;

			case GameState::GameOver:
			{
				if (!gameOverHandleEvents(gameData))
				{
					return false;
				}
			}
		}
	}

	return true;
}
