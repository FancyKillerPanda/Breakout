#include <stdio.h>
#include <chrono>
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

	result.window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

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
	
	paddleReset(gameData.renderer, gameData.paddle);
	ballReset(gameData.renderer, gameData.ball);

	int brickX = SCREEN_WIDTH / 24;
	int brickY = SCREEN_HEIGHT / 10;

	for (Entity& brick : gameData.bricks)
	{
		brick.colour = SDL_Color { 255, 255, 255, 255 };
		brick.texture.rect = { brickX, brickY, BRICK_WIDTH, BRICK_HEIGHT };

		brickX += BRICK_WIDTH + (SCREEN_WIDTH / 12);
	}

	return true;
}

int main(int argc, char* argv[])
{
	GameData gameData = init();
	gameInit(gameData);

	unsigned int frameCounter = 0;
	std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::now();

	while (gameData.running)
	{
		// Calculates delta time
		std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> diff = currentTime - lastTime;
		gameData.deltaTime = diff.count() / 1000.0;
		lastTime = currentTime;

		// Text for frame rate
		frameCounter += 1;
		if (frameCounter % 20 == 0)
		{
			char newFpsText[256];
			sprintf_s(newFpsText, 256, "Breakout V0.1.0 | %dFPS", (int) (1.0 / gameData.deltaTime));
			gameData.fpsText.text = newFpsText;
			updateTextTexture(gameData.renderer, gameData.fpsText);

			frameCounter = 0;
		}

		switch (gameData.gameState)
		{
			case GameState::Gameplay: 
			{
				gameData.running = gameplayHandleEvents(gameData);
				if (gameData.running) gameData.running = gameplayUpdate(gameData);
				gameplayDraw(gameData);
			} break;
		}
	}
	
	return 0;
}
