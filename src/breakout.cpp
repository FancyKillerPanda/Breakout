#include <stdio.h>

#include <SDL/SDL.h>

constexpr int SCREEN_WIDTH = 960;
constexpr int SCREEN_HEIGHT = 540;

struct GameState
{
	bool running = false;
	
	// SDL2 data
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Event event = {};
};

// NOTE(fkp): Returns true on success, false on fail
GameState init()
{
	GameState result = {};
	result.running = true;  // Set to false if init fails
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		printf("Failed to Initialise SDL2");
		result.running = false;
	}

	result.window = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (result.window == nullptr)
	{
		printf("Failed to Create Window");
		result.running = false;
	}

	result.renderer = SDL_CreateRenderer(result.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (result.renderer == nullptr)
	{
		printf("Failed to Create Renderer");
		result.running = false;
	}

	return result;
}

// NOTE(fkp): Returns true if success, false if games needs to exit
bool gameHandleEvents(GameState& gameState)
{
	while (SDL_PollEvent(&gameState.event))
	{
		switch (gameState.event.type)
		{
			case SDL_QUIT:
			{
				return false;
			} break;
		}
	}

	return true;
}

// NOTE(fkp): Returns true if success, false if games needs to exit
// TODO(fkp): Take delta-time
// TODO(fkp): Potentially take game state in an object
bool gameUpdate(GameState& gameState)
{
	// TODO(fkp): Implement game update
	return true;
}

// Draws the game state
// TODO(fkp): Potentially take game state in an object
void gameDraw(GameState& gameState)
{
	SDL_RenderClear(gameState.renderer);
	// Draw code goes here
	SDL_RenderPresent(gameState.renderer);
}

int main(int argc, char* argv[])
{
	GameState gameState = init();

	while (gameState.running)
	{
		gameState.running = gameHandleEvents(gameState);
		if (gameState.running) gameState.running = gameUpdate(gameState);
		gameDraw(gameState);
	}
	
	return 0;
}
